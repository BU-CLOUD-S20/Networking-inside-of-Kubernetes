#! /bin/sh
#
# up.sh
# Copyright (C) 2020 wangfuyao <wangfuyao@ous-MacBook-Pro.local>
#
# Distributed under terms of the MIT license.
#


docker-compose -f docker-compose.yaml up -d
docker start node1
docker start node2
docker start node3
