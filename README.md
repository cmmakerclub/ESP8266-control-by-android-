# ESP8266-control-by-android-
This use ESP8266 like car controller
for last versions Android APP 

 ch1_Eleveltor = b.substring((b.indexOf('a') + 1), b.indexOf('b')).toInt();
      ch2_roll = b.substring((b.indexOf('b') + 1), b.indexOf('c')).toInt();
      ch3_power = b.substring((b.indexOf('c') + 1), b.indexOf('d')).toInt();
      //ch3_power = ch3_power*0.4;
      ch4_yaw = b.substring((b.indexOf('d') + 1), b.indexOf('p')).toInt();
      //
      rollKp = b.substring((b.indexOf('p') + 1), b.indexOf('i')).toInt();
      rollKi = b.substring((b.indexOf('i') + 1), b.indexOf('k')).toInt();
      rollKd = b.substring((b.indexOf('k') + 1), b.indexOf('!')).toInt();
