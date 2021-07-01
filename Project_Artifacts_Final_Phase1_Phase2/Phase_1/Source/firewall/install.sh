#!/bin/bash

sudo install -m 0550 network-firewall /usr/bin/network-firewall
sudo install -m 0440 rule.conf /var/shinpark
sudo install -m 0550 network-firewall.service /lib/systemd/system/
systemctl enable network-firewall.service
