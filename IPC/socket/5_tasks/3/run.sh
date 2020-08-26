#!/bin/bash
echo Running..
gnome-terminal --geometry 80x24-0-0 -e ./server &
gnome-terminal --geometry 80x24+0+0 -e ./client &
gnome-terminal --geometry 80x24-0+0 -e ./client2 &