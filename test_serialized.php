<?php
 $payload = 16;
 $object = new HyperLogLogPlusPlus($payload);
 $result1 = $object->offer("1982732193");
 echo "Result : ",$result1."\n\n";
 echo "To string convertion : ",$str1 = $object->toString()."\n\n";

 $result2 = $object->offer("151823971283712978");
 echo "Result : ",$result2."\n\n";

 echo "To string convertion : ",$str2 = $object->toString()."\n\n";

try{
 	$object2 = new HyperLogLogPlusPlus(65539,$str1);
}
catch(Exception $e){
	echo "" .$e->getMessage()."\n";
}

 $object3 = new HyperLogLogPlusPlus(65539,$str2);

 $result1 = $object2->offer("1518211111712978");
 echo "Result : ",$result1."\n\n";

 $object3->merge($object2);

 echo "Cardinality: ",$object3->count()."\n\n";

 $object3->mergeRaw("890123", 16);
 echo "Cardinality after merging raws: ",$object3->count()."\n\n";

 $object3->mergeRaw("1111111312798", 16);
 echo "Cardinality after merging raws: ",$object3->count()."\n\n";

 $object->mergeRaw("89012371289312798", 65536);
 echo "Cardinality after merging raws in an object which had not been create with serialization: ",$object->count()."\n\n";


?>