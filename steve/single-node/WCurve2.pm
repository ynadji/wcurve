package WCurve2;

use strict;

use Carp;
use Getopt::Long;

use Data::Dumper;
    $Data::Dumper::Purity           = 1;
    $Data::Dumper::Terse            = 1;
    $Data::Dumper::Indent           = 1;
    $Data::Dumper::Deepcopy         = 0;
    $Data::Dumper::Quotekeys        = 0;

########################################################################
# package variables
########################################################################

# catch: nasty rounding bug leaves sin(0) at 10e-16.
# fix is to check for the sin < $zero.
#
#  DB<27> x $sin
#  0  '1.22460635382238e-16'

use constant ZERO => 1e-10;
use constant TINY => 1e-6;

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

our @order = qw( T A G C );

my %cornerz = ();

# allows the caller to set these in a begin block before
# this code is required. after that the lexicals are faster.

my $verbose = 0;
my $debug   = 0;

my @nada = ( 0, 0 );
my $nada = \@nada;

########################################################################
# subroutines
########################################################################

########################################################################
# remember: this needs to be called explicitly for requries.
#
# generate the corners.
# this allows putting off evaluation of the order until after 
# the module is required, which in turn allows the caller to 
# set the order.

sub import
{
    # ignore the module name.

    shift if @_ && $_[0] eq __PACKAGE__;

    # trig values are (sin x) / 2 and (cos x)/2
    # for multiples of pi/2.

    @order = @_[0..3] if @_;

    my @trig =
    (
        # [ (sin $i)/2, (cos $i)/2 ]

        [  0.5,  0.0 ],
        [  0.0,  0.5 ],
        [ -0.5,  0.0 ], 
        [  0.0, -0.5 ]
    );

    # merge the order with the standard trig values.

    %cornerz = map { ( $order[$_], $trig[$_] ) } ( 0..3 );

    ()
}

########################################################################
# subs for file id string, output report info.

sub idstring { join '_', __PACKAGE__, @order }

sub heading { join ' ', __PACKAGE__, 'with', @order }

########################################################################
# read one base at a time and convert it to the next pair
# of ( radius, angle ).
#
# the result is stored in a pre-existing, pre-sized array
# at the given offset.

{
    my $r   = 0;    # polar co-ord's
    my $a   = 0;

    my $x   = 0;    # cartesian co-ord's
    my $y   = 0;

    my $sin = 0;

    my $base    = '';

    sub generate
    {
        %cornerz or import;

        $r = 0;
        $a = 0;

        my $dna = shift
        or croak "Bogus generate: missing dna sequence";

        my @seq = split //, $dna;

        my $cache = shift
        or croak "Bogus generate: missing cache";

        # reset the cache length on the base dna
        # or if the merged sequence is longer.

        $#$cache = $#seq if $offset == 0 || @$cache < @seq;

        # the cache is modified in place, with the successive
        # values assigned to the w-curve sequence.

        for( @seq )
        {
            # cartesian co-ords of the modpoint of a line are
            # at the midpoints along x & y. this will simply be
            # the average of the sides. since one side is fixed
            # a 1, the values in %cornerz are 0 and 0.5, which
            # allows direct addition to 1/2 the values taken
            # from the radius.

            ( $x, $y ) = @{ $cornerz{$_} };

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

            $a = atan2 $y, $x;

            $sin = sin $a;

            $sin = 0 unless abs( $sin ) > ZERO;

            $r = abs( $sin ? $y / $sin :  $x );

            $r = 0 unless $r > TINY;

            $a = 0 unless $r && abs($a) > TINY;

            $_ = [ $r, $a, $_ ];
        }

        # pass back the cache for daisy-chaining.

        $cache
    }

}

########################################################################
# perform the comparison. by the time any calls get down here the 
# two genes have been sanity checked by the pre-generate and generate
# passes. at this point the comparsion just goes full speed.

{
    my $r0 = '';
    my $a0 = '';
    my $r1 = '';
    my $a1 = '';

    my $cos = '';

    my $total = '';

    my $a = [];
    my $b = [];

    ry @a = [];
    my @b = [];

    sub merge { ( $a, $b ) = @_[0,1] }

    sub iterate
    {
        if( @$a || @$b )
        {
            push( @a, shift @$a);
            push( @b, shift @$b);

            ( $a[-1], $b[1] )

        }
        else
        {
            $a = $aa;
            $b = $bb;

            ()
        }
    }

    sub compare
    {
        merge @_;

        $total = 0;

        my $base = '';
        my $alt  = '';

        while( ($base,$alt) = iterate )
        {
            ( $r0, $a0 ) = $base ? @$base : @nada;
            ( $r1, $a1 ) = $alt  ? @$alt  : @nada;

            ( $r0, $r1 ) = ( $r1, $r0 ) if $r1 > $r0;

            $cos = cos( $a0 - $a1 );

            $cos = 0 unless abs( $cos ) > ZERO;

            $total += $r0 - $r1 * $cos;
        }

        $total
    }

        # caller gets back the average score / base.

        $total / @$wcache
    }
}

########################################################################
# used for, er... testing.
#
# $ perl -MWCurve -e 'WCurve::test";

sub test
{
    local $\ = "\n";
    local $, = ' ';
    local $" = ' ';

    local $/;

    $verbose = 99;

    $debug = 0;

    my $cache = [];

    # test simple combinations of corners first.

    print "Testing: @order";

    print "Corners Hash:", Dumper \%cornerz;

    $DB::single = 1;

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
        print "\nTesting string: $string";

        $DB::single = $debug;

        my $score = -1;

        generate $string, 0, $cache;
        generate $string, 1, $cache;

        $score = compare $cache; # should be zero

        print "\nSame Curve:", Dumper $cache, "\nScore: $score";

        generate join( '', reverse( split //, $string)), 1, $cache;

        $score = compare $cache; # should be zero

        print "\nReverse  Curve:", Dumper $cache, "\nScore: $score";

    }

    # test for rouding errors

    $DB::single = 1;

    {
        my $long = join( '', @order) x 131072;

        generate $long, 0, $cache;
        generate $long, 1, $cache;

        my $score = compare $cache; # should be zero

        print "Identical Long: $score";

        generate reverse($long), 1, $cache;

        $score = compare $cache; # should be consistent

        print "Reversed Long: $score";
    }

    $DB::single = 1;

    my @DNA = qw( A T C G );

    for my $i ( 1..200 )
    {
        my $j = $i * 4;

        my $random = join '', map { $DNA[rand @DNA] } (1..$j);

        generate $random, 0, $cache;
        generate $random, 1, $cache;

        my $score = compare $cache; # should be zero

        print "Identical Random  $j: $score";
    }

    $DB::single = 1;

    # view scores from random dna.
    # should approach some sort of average...

    for my $i ( 1..200 )
    {
        my $j = $i * 4;

        generate join( '', map { $DNA[rand @DNA] } (1..$j) ), 0, $cache;
        generate join( '', map { $DNA[rand @DNA] } (1..$j) ), 1, $cache;

        my $score = compare $cache; 

        print "Different Random $j: $score";
    }

    $DB::single = 1;

    for my $i ( 1..1000 )
    {
        my $i = int( 500 + rand 500 );
        my $j = 0.85 * $i;
        my $k = 0.30 * $i;
        my $l = int( $j + rand $k );

        my $a = join( '', map { $DNA[rand @DNA] } (1..$i) );
        my $b = join( '', map { $DNA[rand @DNA] } (1..$l) );

        generate $a, 0, $cache;
        generate $b, 1, $cache;

        my $score1 = compare $cache; 

        generate $a, 1, $cache;

        my $score2 = compare $cache; 

        print "\nDifferent Random $i, $l: $score1";
        print "Identical Random $i: $score2";
    }


}

# keep require happy

1

__DATA__

