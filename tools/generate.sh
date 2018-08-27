#!/bin/bash
KXML_COMPILER=../../../../../kode-mm/kxml_compiler/kxml_compiler 

cp -f eagle.dtd eagle_fixed.dtd
sed -i -e 's/\, compatibility?//g' eagle_fixed.dtd
sed -i -E 's/(.*ENTITY\s%\sInt\s*\")CDATA(\".*)/\1xs:integer\2/g' eagle_fixed.dtd
sed -i -E 's/(.*ENTITY\s%\sReal\s*\")CDATA(\".*)/\1xs:decimal\2/g' eagle_fixed.dtd

perl dtd2xsd.pl eagle_fixed.dtd  > eagle.xsd

$KXML_COMPILER --xml eagle.xsd -d . --verbose
