<?php
 $payload = 16;
 $object = new HyperLogLogPlusPlus($payload);
 $result1 = $object->offer("1982732193");
 echo "Result : ",$result1."\n\n";
 $result2 = $object->offer("151823971283712978");
 echo "Result : ",$result2."\n\n";

 echo "To string convertion : ",$str = $object->toString()."\n\n";

 $object2 = new HyperLogLogPlusPlus($payload);

 $result1 = $object2->offer("1518211111712978");
 echo "Result : ",$result1."\n\n";

 $object3 = new HyperLogLogPlusPlus($payload);
 $result1 = $object3->offer("11517645978");
 echo "Result : ",$result1."\n\n";


 $object3->merge($object2);

 echo "Cardinality: ",$object3->count()."\n\n";

 $object3->mergeRaw($str);

?>