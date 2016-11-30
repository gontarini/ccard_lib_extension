<?php
 $payload = 16;
 $object = new HyperLogLogPlusPlus($payload);
 $result1 = $object->offer("1982732193");
 echo "Result : ",$result1."\n\n";

 echo "To string convertion : ",$str = $object->toString()."\n\n";

 $object2 = new HyperLogLogPlusPlus($payload,$str);

 $result1 = $object2->offer("1518211111712978");
 echo "Result : ",$result1."\n\n";

 $object2->merge($object);
 echo "Objects couldn't have been merged, because they're different size.";
 echo "One created without serialized object has bitmap size = 65536, \n whereas the one created with such object 16 \n\n";

 $object2->mergeRaw($str, 65536);
 echo "Wrong size of merging raw";

 $object2->mergeRaw($str, 16);
 echo "Correct size of merging raw \n";

?>