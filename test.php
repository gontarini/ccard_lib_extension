<?php
 $payload = 16;
 $object2 = new HyperLogLogPlusPlus();
 $object2->_length = $payload;
 $object2->_bitmap = "9876543";
 // echo $object2->_length."\n";
 // echo $object2->_bitmap."\n";
 $object2->init();
 // $object2->initSerialized();
 // echo $object2->offer(155)."\n";
 echo $object2->offer(155812937128397)."\n";
 echo $object2->offer(155812937128397)."\n";

 $str = $object2->toString()."\n"; //smth is wrong
 echo "Objects amount in the context: ";
 echo $object2->count()."\n";

 $object3 = new HyperLogLogPlusPlus();
 $object3->_length = $payload;
 $object3->init();

 // $object2->merge($object3);
$object2->mergeRaw(1231541252131231, 32);

?>