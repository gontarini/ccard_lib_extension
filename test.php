<?php
 $payload = 16;
 $object2 = new HyperLogLogPlusPlus($payload);


 // $object2 -> offer("cos");
 // $value = 2;
 // echo $object2->get_payload();
 $object2->HyperLogLogPlusPlusString("pawel");
 $object2->count();
?>