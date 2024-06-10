# emailapi

### Instalation
```
git clone https://github.com/BarPos/emailapi
cd emailapi
git submodule update --init
mkdir out
cd out
cmake ..
cmake --build .
```

### Add to systemd
Create a service at `/etc/systemd/system/emailapi.service`
```
[Unit]
Description=emailapi
After=network.target

[Service]
Type=exec
Restart=always
RestartSec=1
WorkingDirectory=[email api directory]/out/
ExecStart=[email api directory]/out/emailapi

[Install]
WantedBy=multi-user.target
```
```
systemctl start emailapi
systemctl enable emailapi
```
emailapi will run on port `18081`
to add ssl you need to use a proxy

### Updating
navigate to emailapi directory
```
systemctl stop emailapi
rm -rf out
mkdir out
git pull
git submodule update --init
cd out
cmake ..
cmake --build .
systemctl start emailapi
```

### License
```
MIT License

Copyright (c) 2024 Bartłomiej Postek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
