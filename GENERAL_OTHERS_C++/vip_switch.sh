#!/usr/bin/expect

set timeout 60

set ip [lindex $argv 0]
#set ruleId [lindex $argv 1]
#set vLenId [lindex $argv 2]

set uid "root"
set pwd "movik123"

if {$ip == ""} {
    exit
}

spawn ssh -l $uid $ip

expect {
     -regexp {.*assword} {
             set out $expect_out(buffer)
             exp_send $pwd
             exp_send "\n"
             exp_continue
     }
     -regexp {.*continue connecting.*} {
             exp_send "yes\r"
             exp_continue
     }
     -regexp {.*#} {
             set out $expect_out(buffer)
         interact
     }
     -regexp {.*>} {
             set out $expect_out(buffer)
             exp_send "/opt/srstackware/bin/imish\n"
     }
}

expect {
    -regexp {.*>} {
        exp_send "show interface > interface.txt"
        exp_send "\n"
    }
}

expect {
    -regexp {.*>} {
        set out $expect_out(buffer)
        exp_send "exit\n"
    }
}

expect {
    -regexp {.*root>} {
        exp_send "scp interface.txt root@192.168.121.10:/root"
        exp_send "\n"
    }
}

expect {
     -regexp {.*assword} {
             exp_send "movik123"
             exp_send "\n"
     }
}

expect {
    -regexp {.*>} {
        set out $expect_out(buffer)
        exp_send "exit\n"
    }
}

#        exp_send "cat /root/interface.txt | grep -v "Hardware" | grep -v "Current" | grep -v "Physical" | grep -v "Description" | grep -v "Bandwidth" | grep -v "VRF Binding" | grep -v "index" | grep -v "VRRP" | grep -v "input packets" | grep -v "output packets" | grep -v "inet" > /root/face.txt"


