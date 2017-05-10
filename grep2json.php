<?php

// grep2json.php
// L S Ng lsn95r@gmail.com 2017-03-14

var_dump($argv);

$a=file($argv[2]);

$b=preg_grep('/'.$argv[1].'/', $a);

var_dump($b); // $b is the original results
$bt=$b; // $bt is the tree for additional results

foreach($b as $k => $c) {
preg_match_all('/'.$argv[1].'/', $c, $m, PREG_OFFSET_CAPTURE);

$j=array();

$j[$k]['m']=$m;

echo "\n".$k." ".count($m[0])." ";
foreach($m[0] as $kc => $d) {
echo $m[0][$kc][1]." ";
}
echo " | ";

$t = preg_split('/ /', $c, -1, PREG_SPLIT_OFFSET_CAPTURE);

$j[$k]['t']=$t; // json style string index

foreach($t as $kt => $ta) {
echo $ta[1]." ";
}
echo "\n";

$bt[$k]=$j[$k];

// Store tree in memory first, can manipulate, or output to file?

}

echo graph($bt, $argv[3])."\n";

echo json_encode($bt)."\n";

function graph($t, $s)
{

// use string before first / as command or flags

if ($s=='' || $s=='/') return json_encode(array_keys($t), JSON_PRETTY_PRINT);
else {

$sa = explode('/', $s);

if (count($sa)==2) {
if ($sa[0]=='k') return json_encode(array_keys($t[$sa[1]])); else
 return json_encode(($t[$sa[1]]), JSON_PRETTY_PRINT);
}
else if (count($sa)==3) {
$ta=($t[$sa[1]]);

return json_encode(($ta[$sa[2]]), JSON_PRETTY_PRINT);
}
else if (count($sa)>3) {

$ta=$t;
foreach($sa as $sk => $sb) {
if ($sk>0) {
$ta=($ta[$sb]);

echo "\n\n".__LINE__." $sk ".json_encode(($ta), JSON_PRETTY_PRINT);
}

}
echo "\n";
}

}

}
