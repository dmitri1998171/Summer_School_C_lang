#!/bin/bash
echo Running..
konsole -geometry 60x25 -e ./server-fork &
konsole -geometry 60x25+1000+1000 -e ./client &
konsole -geometry 60x25+500+10 -e ./client2 &