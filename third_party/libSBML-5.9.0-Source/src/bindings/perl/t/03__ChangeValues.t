# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl GRN-Models.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 2 };

use LibSBML;

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

my $d = new LibSBML::SBMLDocument(2,3);
my $m = $d->createModel();
$m->setId('xtof');
my $s = $m->createSpecies();
$s->setId('rainer');
my $ref = join '', <DATA>;
my $doc = $d->writeSBMLToString();
ok($doc, $ref);

$m->setId('flummi');
$s = $m->getSpecies('rainer');
$s->setId('schmutzer');
my $doc2 = $d->writeSBMLToString();
ok($doc2 ne $ref);

__DATA__
<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level2/version3" level="2" version="3">
  <model id="xtof">
    <listOfSpecies>
      <species id="rainer"/>
    </listOfSpecies>
  </model>
</sbml>
