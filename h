<?php

/*

PAGE (Programming As Graph Editing) demo

Multiple Graph Parsing: h = html parser

2017-05-03


Derived from g:
$ php g ja e/81/t/5 doremi | php g . p/81/t/5
json_pretty
[
    "demo_boxstack_a$(EXEEXT)",
    "doremi"
]


*/
// L S Ng lsn95r@gmail.com 2017-03-17

// no path view keys of root
// / view full root
// j/ view as json (not implemented)
// p/ pretty print json

// maintain table of command mapped to function source file $FLUT

include_once("simple_html_dom.php");

if ($argv[1]=='.') $a=file_get_html("php://stdin");
else $a=file_get_html($argv[1]);

$h=$a;

$hr=$h->find('html');

echo $hr[0]->tag;

$c=$hr[0]->children();

foreach($c as $e) echo " ".$e->tag;

$cb=$c[1]->children();

foreach($cb as $e) echo " ".$e->tag;

// exit;

// $b=json_decode($a[0], TRUE);

// $rb=&$cb;
$rb=&$hr[0];
echo gcmd($rb, $argv)."\n"; // array('$n',0) was added

// 20170317 need to print json of full tree because command only can return json of branch


exit;

function sequence(&$t, $n)
{

echo __FUNCTION__." ".__LINE__."\n";

var_dump($t);
var_dump($n);

$t[0]['p']=-1;
$t[0]['n']=1;

foreach($t as $kt => $ta) {
// $ta['p']=$kt-1; // cannot reassign to other variable
// $ta['n']=$kt+1; // need to use $t directly
$t[$kt]['p']=$kt-1;
$t[$kt]['n']=$kt+1;
}


}


function add_element(&$t, $n)
{

// flag for FLUT? add node from json file? or json string?
// multiple arguments? make multiple arguments into json, then add
// f jsonfile -- 2 arguments open json file. 
// json string treat as one argument '{   }'

// 20170319 if new element is json, it can be scalar (one string, with json string index) or array (multiple elements)
// if json is scalar (string), then can add_element or add_node, otherwise add_node
// add element = add (array or element) to end of existing array (append): add in array as element [ a b c .... x ] add in array as array [ a b c .... [x] ]
// as x (json array) without making another array outside i.e. [ x ] or  [ [ . ] ] 
// json_decode gives array, no need to break the array, just add to end, as array has no sequqence
// add node = add new node, also end of existing array? add outside array (in parent's end) [ a b c .... ] [ x ] as array or as element
// [ a1 a2 ... ] need to append [ s1 s2 ... ] as

// [ a1 a2 ... s1 s2 ... ] or add_element flag= break array
// [ a1 a2 ... [ s1 s2 ... ] ] or add_element, if string, add string as element, or add as array?

// use flag: php g ja e/addr j {} json
// use flag: php g ja e/addr f json file
// e_s/addr split array before storing

// no flag: php g ja e/addr string
// use flag: php g ja e_a/addr string; store string as array of string
// 
// flag after address is input flag
// flag after command e_a is to define how to process input 

// [ a1 a2 ... ] [ s1 s2 ... ] add to parent; if root /, has no parent, just append
// same flags as above

$l=count($n);

// echo __LINE__." "; var_dump($n[2]);

$sa = explode('/', $n[2]);
// echo __LINE__." "; var_dump($sa);
$c=strpos($sa[0], "_"); // var_dump($c);

if ($l==5 && $n[3]=='f') {
    $a=file($n[4]);
    $b=json_decode(trim($a[0]),true);
    // echo __LINE__." "; var_dump($a);
    if ($c!==false) {
        $d = explode("_", $sa[0]);
        if ($d[1]=='s') foreach($b as $bk => $be) $t[$bk]=$be; 

    }
    else $t[]=$b;
}
else if ($l==5 && $n[3]=='j') { 
    $b=json_decode($n[4],true);
    // echo __LINE__." "; var_dump($a);
    if ($c!==false) {
        $d = explode("_", $sa[0]);
        if ($d[1]=='s') foreach($b as $bk => $be) $t[$bk]=$be; 
    }
    else $t[]=$b;
} // j json-string
else if (($c=strpos($sa[0], '_'))!==false) {  // echo __LINE__." "; var_dump($c); 
    $d = explode("_", $sa[0]);
    if ($d[1]=='a' && $n[3]!='') $t[]=array($n[3]); 

} // e_a add as array
else if ($n[3]!='') $t[]=($n[3]);
// else $t[]=($n[3]); // if json need to parse? or just make it to file

return json_encode($t); // return to gcmd, this function has no access to tree, gcmd need to return original tree

}


function add_node(&$t, $n)
{

// flag for FLUT? add node from json file? or json string?
// multiple arguments? make multiple arguments into json, then add
// f jsonfile -- 2 arguments open json file. 
// json string treat as one argument '{   }'

echo __FUNCTION__." ".__LINE__."\n";

$l=count($n);

if ($l==5 && $n[3]=='f') {
    $a=file($n[5]);
    $b=json_decode($a[0],true);
    $t[]=$b;
}
else $t[]=array($n[3]); // if json need to parse? or just make it to file?

return json_encode($t);

}

function rootkeys($t)
{

echo __FUNCTION__."\n";

return json_encode(array_keys($t), JSON_PRETTY_PRINT);;
}

function view_keys($t)
{

echo __FUNCTION__."\n";

return json_encode(array_keys($t), JSON_PRETTY_PRINT);;
}

function json_pretty($t)
{

echo __FUNCTION__."\n";

return json_encode(($t), JSON_PRETTY_PRINT);;
}


function view_json($t)
{

echo __FUNCTION__."\n";

return json_encode(($t));;
}

function children($t)
{

echo __FUNCTION__."\n";

        $tc=array();
        foreach($t->children() as $k => $c) {
            $tc[] = array($k, $c->tag);
        }
        return json_encode($tc);


// return $t->outertext; //json_encode(($t));;
}



function gcmd(&$t, $s)
{

// use string before first / as command or flags
// read additional commands from external file in future

$FLUT=array('v'=>"view_json",'k'=>"view_keys",'p'=>"json_pretty",'a'=>"add_node",
            'e'=>"add_element", 'c'=>"children");

// echo __FUNCTION__." ".__LINE__." ".json_encode($t)."\n";
// $t[]='a'; // add successful, but branch of a tree is a copy, modifying the copy does not change the original?


// $s: command line
// $sa: URI

// Assume / is body. /html /header as exception.

$D=1;
if ($D) echo "\n".__LINE__." ".count($s)."\n";

if (count($s)<3) { $func="rootkeys"; $addr=$t; $arg=$s; return $t->outertext; // call_user_func($func, $addr, $arg); 
}
else {

$sa = explode('/', $s[2]);

if ($sa[0]!='') { 
    // 20170319 add command_flag 
    $sb = explode('_', $sa[0]);
    // echo __LINE__." "; var_dump($sb);
    if (count($sb>1)) { $func=$FLUT[$sb[0]]; } // need to define FLUT for command 'c' 
    else $func=$FLUT[$sa[0]]; 
}     // 20170317: separate logic of command and address
else { $func=$FLUT['v']; }

if ($sa[1]=='') { 

if ($D) echo "\n".__LINE__." ".count($s)."\n";

    if ($sa[0]=='p') return json_encode(($t), JSON_PRETTY_PRINT);
    else if ($sa[0]=='k') return json_encode(array_keys($t), JSON_PRETTY_PRINT);

    else if ($sa[0]=='c') { 
	$tc=array(); 
	foreach($t->children() as $k => $c) { 
	    $tc[] = array($k, $c->tag); 
	} 
	return json_encode($tc); 
    }

    else return $t->outertext; 
    // 20170506 need to enumerate DOM elements
    
    // json_encode(($t));
    // $func=$FLUT[$sa[0]] command mapped to function names
}

else if (count($sa)==2) {

    if ($sa[0]=='k') { $addr=$t[$sa[1]]; return json_encode(array_keys($t[$sa[1]])); }
    else if ($sa[0]=='p') return json_encode(($t[$sa[1]]), JSON_PRETTY_PRINT);    
    else if ($sa[0]!='') {

if ($D) echo "\n".__LINE__." ".count($s)." $sa[1]\n";
 
	$func=$FLUT[$sa[0]]; // 20170508 default process for command options

	$ta=$t->children();
        $addr=&$ta[$sa[1]]; }   // must pass & reference in $addr
    else { $func=$FLUT['v']; $addr=&$t[$sa[1]]; // echo __LINE__."\n"; 
    }
    // shoud get address then pass to processing function, including view
    // view keys, or view what, or process what?
    // save addresses in address decoding part (here), call processing function at the end.
    
    // 20170317: separate logic of command and address
}
else if (count($sa)==3) {
    
    $ta=&$t[$sa[1]]; // (&$t[$sa[1]]) does not work?

    $addr=&$ta[$sa[2]];    
    // $addr=$r;
    
    /* above and below has different effect!!
       $r=&$ta[$sa[2]];    
       $addr=$r; */
    
}
else if (count($sa)>3) {

    // $ta=&$t; // initial assignment need & !! but  $ta=($ta[$sb]) overrides original tree!!
    
    $ta=&$t[$sa[1]]; // (&$t[$sa[1]]) does not work?
    foreach($sa as $sk => $sb) {

    if ($sk>1) {
        $ta = &$ta[$sb]; // undocumented behaviour of & reference to array?
        // $ta=($ta[$sb]);
        $addr=&$ta;
    }
}
}


}

$arg=$s;
$C=$sa[0];

// p,k,v return partial view format, other commands return full tree
if ($C=='p' || $C=='k' || $C=='v' || $C=='' || $C=='c' ) { // echo __LINE__."\n"; 
    return call_user_func($func, $addr, $arg); }
else {
    call_user_func($func, $addr, $arg);
    return json_encode($t); 
}

}
