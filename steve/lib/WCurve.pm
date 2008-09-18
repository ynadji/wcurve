package WCurve;

use strict;

use Carp;

use Benchmark   qw( :hireswallclock timestr timediff );
use Symbol      qw( qualify_to_ref );

use Data::Dumper;

my $switch
= sub
{
    $_[2] ? @_[1,0] : @_[0,1]
};

use overload
(
    q{<=>}   =>
    sub
    {
        my ( $a, $b ) = &$switch;

        ( $a + 0 ) <=> ( $b + 0 )
    },

    q{>}    =>
    sub
    {
        my ( $a, $b ) = &$switch;

        ( $a + 0 ) > ( $b + 0 )
    },

    q{<}    =>
    sub
    {
        my ( $a, $b ) = &$switch;

        ( $a + 0 ) < ( $b + 0 )
    },

    q{+}    =>
    sub
    {
        my( $a, $b ) = &$switch;

        $a->[1] + ( 0 + $b )
    },

    q{bool} =>
    sub
    {
        defined $_[0]
    },

    q{0+}   =>
    sub
    {
        # the length

        $_[0]->[1]
    },

    q{""}   =>
    sub
    {
        # whatever the caller stored there...

        $_[0]->[2] || ''
    },
);

use vars qw( $worst_case $ZERO $TINY );

########################################################################
# package variables
########################################################################

BEGIN
{
    # Constants:

    # badness per base cannot get worse than this with 
    # a 1x1 square.
    #
    # catch: nasty rounding bug leaves sin(0) at 10e-16.
    # fix is to check for the sin < $zero.
    #
    #  DB<27> x $sin
    #  0  '1.22460635382238e-16'

    for
    (
        [ worst_case => \2.0         ],
        [ ZERO       => \ 2 ** -30   ],
        [ TINY       => \ 2 ** -18   ],
    )
    {
        my( $name, $value ) = @$_;

        my $ref  = qualify_to_ref $name, __PACKAGE__;

        *$ref = $value;
    }
}

# limit for total error before the comparison is rejected.

my $limit   = $worst_case;

# angles of the corners, their radii are all 1.0.
# c & g are opposites, t & a are opposites. 
# the corners are all on the x/y axis at [ 1,0 ],
# [0,1], [-1,0], [0,-1] (i.e., each vertex is +/-1
# and zero).
#
# assigning the slice handles uppercase base values.
#
# offsetting the rows makes adjusting the corners
# easier via cut+paste lines.

my %cornerz
= do
{
    my @order = qw( T A G C );

    my @trig =
    (
        # [ (sin $i)/2, (cos $i)/2 ]

        [  0.5,  0.0 ],
        [  0.0,  0.5 ],
        [ -0.5,  0.0 ], 
        [  0.0, -0.5 ]
    );

    # merge the order with the standard trig values.

    map
    {
        (
            ( uc $order[$_],    $trig[$_] ),
            ( lc $order[$_],    $trig[$_] ),
        )
    }
    ( 0..3 )
};

########################################################################
# read one base at a time and convert it to the next pair
# of ( radius, angle ).
#
# the result is stored in a pre-existing, pre-sized array
# at the given offset.

sub generate
{
    my $proto   = shift
    or croak "Bogus generate: missing prototype";

    my $seq     = shift
    or croak "Bogus generate: missing sequence";

    # anything left in @_ is extra data the caller
    # wants stuck into the head node.

    my $node    = [];
    my $head    = [ $node, length $seq, @_ ];

    my $r   = 0;
    my $a   = 0;

    my $x   = 0;
    my $y   = 0;

    my $sin = 0;

    my $count   = length $seq;
    my $i       = 0;

    my $base    = '';

    eval 
    {
        for( 1 .. $count )
        {
            # cartesian co-ords of the modpoint of a line are
            # at the midpoints along x & y. this will simply be
            # the average of the sides. since one side is fixed
            # a 1, the values in %cornerz are 0 and 0.5, which
            # allows direct addition to 1/2 the values taken
            # from the radius.

            $base       = substr $seq, 0, 1, '';

            ( $x, $y ) = @{ $cornerz{ $base } };

            $x += ( $r * cos $a ) / 2;
            $y += ( $r * sin $a ) / 2;

            # update the angle to the arctan of y/x.
            # perl's atan2 avoids problems where $x is zero.
            #
            # taking the cos w/ sin $a == 0 handles
            # the sign automatically.
            #
            # if the sin is zero then the cos is 1,
            # at which point the new radius is simply
            # the x value.

            $a      = atan2 $y, $x;

            $sin    = sin $a;

            $sin    = 0 unless abs( $sin ) > $ZERO;

            $r      = abs( $sin ? $y / $sin :  $x );

            $r      = 0 unless $r > $TINY;

            $a      = 0 unless $r and abs($a) > $TINY;

            ( $node )  = @$node = ( [], $r, $a );
        }
    };

    die "\nFatal: '$base'\n$seq"
    if $@;

    # caller gets back head of the list, with the count and 
    # pointer to head of the list.

    bless $head, ref $proto || $proto
}

# avoid problems due to long sequences blowing up in 
# perl's recursive deallocator.

DESTROY
{
    my $head = shift;

    $head    = delete $head->[0] while $head;

    return
}

########################################################################
# compare two linked lists

sub limit
{
    my $curve   = shift;

    if( my $newlim  = shift )
    {
        $newlim <= $worst_case
        or die "Bogus new limit: $newlim > max ($worst_case)";

        $newlim > $TINY
        or die "Bogus new limit: $newlim too small";

        $limit      = $newlim;
    }

    print STDERR "\nLimit: $limit\n";

    $limit
}

sub compare
{
    # $wc0 => shorter of the two lists.

    my ( $wc0, $wc1 )   = @_;

    my $tag     = "$wc0, $wc1";

    ( $wc0, $wc1 ) = ( $wc1, $wc0 )
    if $wc1 > $wc0;

    my $short   = $wc1 + 0;
    my $count   = $wc0 + 0;
    my $extra   = $count - $short;

    my $cutoff  = $limit * $count;
    my $total   = 0.0;
    my $diff    = 0.0;

    my ( $r0, $a0 ) = ( 0, 0 );
    my ( $r1, $a1 ) = ( 0, 0 );

    my $cos         = 0;

    $_ = $_->[0] for $wc0, $wc1;

    my $i   = 0;

    for( 1 .. $short )
    {
        last if $total > $limit;

        ( $wc0, $r0, $a0 ) = @$wc0;
        ( $wc1, $r1, $a1 ) = @$wc1;

        ( $r0, $r1 ) = ( $r1, $r0 ) if $r1 > $r0;

        $cos    = cos( $a0 - $a1 );

        $cos    = 0 unless abs( $cos ) > $ZERO;
        
        # rounding errors can cause the 
        # difference to run negative. only
        # fix is to round it to zero in 
        # cases where it's too close to call.

        $diff   = $r0 - $r1 * $cos;

        $diff   = 0 if $diff < $TINY;

        $total += $diff;

        ++$i;
    }

    for( 1 .. $extra )
    {
        last if $total > $limit;

        ( $wc0, $r0 ) = @$wc0;

        $total += $r0;

        ++$i;
    }

    $total / $count
}

########################################################################
# used for, er... testing.
#
# $ perl -MWCurve -e 'WCurve::test";

sub test
{
    select STDERR;

	local $\;
	local $,;
	local $" = ' ';

	local $/;

    local $Data::Dumper::Purity           = 1;
    local $Data::Dumper::Terse            = 1;
    local $Data::Dumper::Indent           = 1;
    local $Data::Dumper::Deepcopy         = 0;
    local $Data::Dumper::Quotekeys        = 0;

	# test simple combinations of corners first.

	my @order = qw( C A T G );

	print "Testing: @order\n";

	print "Corners Hash:", Dumper \%cornerz, '';

	for my $string
	(
		join('', $order[0]) x 8,
		join('', $order[1]) x 8,
		join('', $order[2]) x 8,
		join('', $order[3]) x 8,

		join('', $order[0], $order[2]),
		join('', $order[1], $order[3]),
		join('', $order[0], $order[1]),
		join('', $order[1], $order[2]),
		join('', $order[2], $order[3]),
		join('', $order[3], $order[0]),

		join('', $order[0], $order[2]) x 4,
		join('', $order[1], $order[3]) x 4,
		join('', $order[0], $order[1]) x 4,
		join('', $order[1], $order[2]) x 4,
		join('', $order[2], $order[3]) x 4,
		join('', $order[3], $order[0]) x 4,

		join('', @order) x 2,
		join('', reverse @order) x 2,

	)
	{
        my $altstr  = join '', reverse split //, $string;

		print "\nTesting string: $string ($altstr)\n";

        my $curve   = __PACKAGE__->generate( $string );
        my $alt     = __PACKAGE__->generate( $altstr );

        my $score0  = $curve->compare( $curve );
        my $score1  = $curve->compare( $alt   );

        print "Compare:  $score0 ( $score1 )\n";
	}

	my @dna = qw( A T C G );

	# view scores from random dna.
	# should approach some sort of average...

    print "\nRandom strings:\n";

    my $count   =  0;
    my $sum     =  0;
    my $sumsq   =  0;
    my $min     =  3;
    my $max     = -1;

    my $gentime =  0;
    my $cmptime =  0;

    my $t0      = Benchmark->new;

    my $a       = ' ' x $max;
    my $b       = ' ' x $max;

    my $i       = '';
    my $j       = '';

    for my $length ( 2 ** 10  .. 2 ** 12  )
    {
        print $length, "\r";

        $a  = join '', map { $dna[ rand @dna ] } ( 1 .. $length );
        $b  = join '', map { $dna[ rand @dna ] } ( 1 .. $length );

        $gentime
        += do
        {
            my $t0  = Benchmark->new;

            $a      = __PACKAGE__->generate( $a );

            my $t1  = Benchmark->new;

            $t1->[0] - $t0->[0]
        };


        $b      = __PACKAGE__->generate( $b );

        $cmptime
        += do
        {
            my $t0  = Benchmark->new;

            $i      = $a->compare( $b );

            my $t1  = Benchmark->new;

            $t1->[0] - $t0->[0]
        };

        $j  = $b->compare( $a );

        $i == $j or warn "Oddity: $i != $j\n";

        ++$count;

        $sum    += $i;

        $min    = $i if $i < $min;
        $max    = $i if $i > $max;
    };

    my $t1      = Benchmark->new;

    my $wall    = $t1->[0] - $t0->[0];

    my $runhz   = $count / $wall    ;
    my $genhz   = $count / $gentime ;
    my $cmphz   = $count / $cmptime ;

    my $avg     = $sum  / $count;

    printf "\nTotals: x => $sum, x^2 => $sumsq, n => $count\n";
    printf "\nComparison score: %0.3f < %.3f < %.3f\n", $min, $avg, $max;
    printf "\nGenerate Hz: %4d\n", $genhz;
    printf "\nCompare Hz:  %4d\n", $cmphz;
}

# keep require happy

1

__DATA__
