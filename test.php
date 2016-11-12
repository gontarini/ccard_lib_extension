<?php
 $payload = 15;
 $object2 = new HyperLogLogPlusPlus($payload);
 echo $object2 ->count();
 echo "";

 $str = "Pawel";
 $object2 -> offer("cos");
 #$value = 2;
 #$object2->get_payload($value);
?>