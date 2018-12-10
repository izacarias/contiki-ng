A minimal Contiki-NG example based on "Hello, world" code.

To make the project

```bash
make TARGET=openmote-cc2538 savetarget
make hello-scott
```

Put the Mote on bootloader mode connecting the ON/SLEEP pin to the GND.

```bash
make hello-scott.upload PORT=/dev/tty.usbserial-A9040YUL
make PORT=/dev/tty.usbserial-A9040YUL login
```

Look for the node's global IPv6, e.g.:
```
[INFO: Native    ] Added global IPv6 address fd00::302:304:506:708
```

And ping it (over the tun interface):
```
$ ping6 fd00::302:304:506:708
PING fd00::302:304:506:708(fd00::302:304:506:708) 56 data bytes
64 bytes from fd00::302:304:506:708: icmp_seq=1 ttl=64 time=0.289 ms
```
