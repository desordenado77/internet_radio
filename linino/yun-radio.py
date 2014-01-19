
from __future__ import print_function
import urllib
import urllib2
import subprocess
import sys
import time
import select
import json

from grooveshark import Client
from grooveshark.classes import Radio


def rest_interface_put(key, value):
    url_name='http://localhost/data/put/'
    urllib2.urlopen(url_name + key + '/' + value)
#    print(url_name + key + '/' + value)

def rest_interface_get(key):
# $ url -u root:arduino http://192.168.1.208/data/get/counter
# {"value":"727091","key":"counter","response":"get"}
    url_name='http://localhost/data/get/'
    response = urllib2.urlopen(url_name + key)
    decoded = json.load(response)
#    print(decoded['value'])
    response.close()
    return decoded['value']
        


def load_stream(proc, song):
    cmd = 'L ' + song.stream.url +'\n'
    rest_interface_put('nowPlaying_song', urllib.quote(unicode(song.name).encode("utf-8")))
    rest_interface_put('nowPlaying_album', urllib.quote(unicode(song.album).encode("utf-8")))
    rest_interface_put('nowPlaying_artist', urllib.quote(unicode(song.artist).encode("utf-8")))
    rest_interface_put('nowPlaying_pos', '0')
    rest_interface_put('nowPlaying_dur', '0')
#    print(cmd)
    proc.stdin.write(cmd)

def pause_stream(proc):
    cmd = 'P\n'
#    print(cmd)
    proc.stdin.write(cmd)

def stop_stream(proc):
    cmd = 'C\n'
#    print(cmd)
    proc.stdin.write(cmd)

def set_volume(proc, val):
    cmd = 'V ' + val + '\n'
#    print(cmd)
    proc.stdin.write(cmd)



client = Client()
client.init()

users = open('gs_users', 'r+')
playlist = open('gs_playlists', 'r+')
radios = open('gs_radios', 'r+')

# users
# user0name...userNname
# user0songs
#       user0song0...user0songM
nusers = 0
nsongs = 0
for line in users:
    user_elements = line.split()
    print(line)
    for song in client.collection(user_elements[0]):
#        print(unicode(song.name).encode("utf-8"))
        rest_interface_put('user' + str(nusers) + 'song' + str(nsongs),  urllib.quote(unicode(song.name).encode("utf-8")))
        nsongs+=1
    rest_interface_put('user' + str(nusers) + 'songs',  str(nsongs))
    rest_interface_put('user' + str(nusers) + 'name',  user_elements[1])
    nusers+=1

rest_interface_put('users',  str(nusers))

# playlists
# playlist0name
# playlist0songs
#   playlist0song0...playlist0songM
nplaylists = 0
nsongs = 0
for line in playlist:
    print(client.playlist(line).name)
    for song in client.playlist(line).songs:
#        print(unicode(song.name).encode("utf-8"))
        rest_interface_put('playlist' + str(nplaylists) + 'song' + str(nsongs), urllib.quote(unicode(song.name).encode("utf-8")))
        nsongs+=1
    rest_interface_put('playlist' + str(nplaylists) + 'songs', str(nsongs))
    rest_interface_put('playlist' + str(nplaylists) + 'name', urllib.quote(unicode(client.playlist(line).name).encode("utf-8")))
    nplaylists+=1

rest_interface_put('playlists', str(nplaylists))

#
# radios
# radio0name 
nradios = 0
radio = ""
for line in radios:
    radio_element = line.split()
#    print(line)
    rest_interface_put('radio' + str(nradios) + 'name', urllib.quote(unicode(radio_element[1]).encode("utf-8")))
    nradios+=1
    radio = radio_element[0]

rest_interface_put('radios', str(nradios))

current_pos = 0
song_duration = 0

print(rest_interface_get('radio0name'))

# mpg123 Commands:
#   L: load file
#   P: pause playback
#   S: stop
#   V: volume (%)
mpg123_proc = subprocess.Popen(['mpg123', '-R'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
for song in client.radio(radio):
    prev_pos = -1
    load_stream(mpg123_proc, song)

    while mpg123_proc.stdout:
        readable = select.select([mpg123_proc.stdout], [], [], 1)[0]
        for s in readable:
            mpg123_line = mpg123_proc.stdout.readline() 
#            print(mpg123_line)
            eos = mpg123_line.find('@P 0')
            pos = mpg123_line.find('@F')
            
            if (pos != -1) :
                current_pos = int(float(mpg123_line.split()[3]))
                if song.duration == 0:
                    song_duration = current_pos + int(float(mpg123_line.split()[4]))
                else:
                    song_duration = song.duration                    
#                print(current_pos + " - " + str(song.duration))

            if (eos != -1) :
                break
        if prev_pos != current_pos :   
#            print(str(current_pos) + " - " + str(song_duration))
            rest_interface_put('nowPlaying_pos', str(current_pos))
            rest_interface_put('nowPlaying_dur', str(song_duration))
            prev_pos = current_pos
        if eos!=-1 :
            break;
