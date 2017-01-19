# serialization-benchmarks
Some simple experiments on encoding binary data with libbson and Rapidjson in C(++).

Output from the main executable:
```
Some experiments on schema-less serialization. Comparing BSON (libbson) with RapidJSON.
The 'Basic' usecase is a simple JSON object with tree key/value pairs:
  {"a":1,"hello":"world","bool":true}
The 'Binary' usecase encodes a simple string key/value pair and 6 MB of data.

ws = wire size. These is the amount of bytes that would have to be transferred over the wire. This is the length of the actual encoded data.

BSON Basic                              ws: 36              time(s): 1.5118e-05
RapidJSON basic                         ws: 35              time(s): 2.801e-05
BSON Binary                             ws: 6291496         time(s): 0.00383603
RapidJson Binary                        ws: 6291490         time(s): 0.152195
RapidJson + base64_encode(binary)       ws: 8388642         time(s): 0.348549
```

## Build
This is a cmake based project. Create a build/ folder and call 'cmake .. && make && ./tests' from it

## Notes
Please note that libbson has been configured and put into this project directly. 
This comes at cost: The configuration in bson-config.h might not be suitable for your machine.
It's recommended to configure & install libbson as a library when using it.

The choosen methods to encode data might not be perfect.
Comments on that are highly encouraged.
