opcontrol --dump
#opreport --callgraph=5 --symbols /opt/universalcache/bin/npconductor  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt --debug-info
#opreport --symbols=/opt/universalcache/bin/npconductor  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt 
#opreport --symbols=/opt/universalcache/bin/conductor  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt 
#opreport --symbols=/home/PKG/bin/conductor  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt --debug-info --callgraph=2
opreport --symbols=/opt/universalcache/bin/conductor  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt --debug-info --callgraph=3
#opreport --callgraph=5 --symbols=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples/a.out  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt 
#opreport --symbols=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples/a.out  --session-dir=/home/MSD/POCO_LEARNING/poco_examples/pocossl/examples --output-file=conductor.txt 
opcontrol --reset
opcontrol --shutdown
