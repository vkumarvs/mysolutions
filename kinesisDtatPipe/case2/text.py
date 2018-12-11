Streaming The Data And Consuming It

Unix watch command which will produce a data every 2 seconds:
watch “python producer”

In another terminal, if you start the consumer app, you will see that the data sent will be catched in a fast way by the consumer
python consumer.py
