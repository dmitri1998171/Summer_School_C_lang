#!/bin/bash
echo Running..
konsole -geometry 60x25+1000+10 -e ./server &
konsole -geometry 60x25+1000+1000 -e ./client1 &
konsole -geometry 60x25+500+10 -e ./client2 &