
from __future__ import print_function
import urllib
import urllib2
import subprocess
import sys
import time
import select
import json
import os
import random
import types

from grooveshark import Client
from grooveshark.classes import Radio

# global variables
# set linino to 0 to use on PC
linino = 1
path = './'
command_end = '\n'
users_array = []
playlist_array = []
radios_array = []
shuffle = 0


# functions

import unicodedata
def strip_accents(s):
   return ''.join(c for c in unicodedata.normalize('NFD', s) if unicodedata.category(c) != 'Mn')


def rest_interface_put(key, value):
    global linino
    url_prefix='http://localhost/data/put/'
    url = url_prefix + key + '/' + urllib.quote(value)
    if linino:
        urllib2.urlopen(url)
    else:
        print(url)

def rest_interface_get(key):
# $ url -u root:arduino http://192.168.1.208/data/get/counter
# {"value":"727091","key":"counter","response":"get"}
    global linino
    url_prefix='http://localhost/data/get/'
    url = url_prefix + key
    if linino:
        response = urllib2.urlopen(url)
        decoded = json.load(response)
#    print(decoded['value'])
        response.close()
        return decoded['value']
    else:
        ret = ''
        try:
            keyfile = open('REST/' + key, 'r')
            ret = keyfile.readline()
            keyfile.close()
            os.remove('REST/' + key)
        except IOError:
            pass

        return ret
        
        


def load_stream(proc, song):
    cmd = 'L ' + song.stream.url +'\n'
    rest_interface_put('nowPlaying_song', (strip_accents(unicode(song.name)).encode("utf-8")))
    rest_interface_put('nowPlaying_album', (strip_accents(unicode(song.album)).encode("utf-8")))
    rest_interface_put('nowPlaying_artist', (strip_accents(unicode(song.artist)).encode("utf-8")))
    rest_interface_put('nowPlaying_pos', '0')
    rest_interface_put('nowPlaying_dur', '0')
#    print(cmd)
    proc.stdin.write(cmd)

def pause_stream(proc):
    cmd = 'P\n'
#    print(cmd)
    proc.stdin.write(cmd)

def stop_stream(proc):
    cmd = 'S\n'
#    print(cmd)
    proc.stdin.write(cmd)

def restart_stream(proc):
    cmd = 'K 0\n'
    proc.stdin.write(cmd)

def set_volume(proc, val):
    cmd = 'V ' + val + '\n'
#    print(cmd)
    proc.stdin.write(cmd)


def try_ping():
    try:
        urllib2.urlopen('http://www.grooveshark.com')
        if linino:
            urllib2.urlopen('http://localhost/data/get/')
    except urllib2.URLError, err:
        print("Connection error " + str(err.reason))
        return 1
    return 0


def random_iter(seq, start):    
    shuffled = list(seq)
    first_item = shuffled[start]
    random.shuffle(shuffled)
    first_item_index = shuffled.index(first_item)
    shuffled[first_item_index] = shuffled[0]
    shuffled[0] = first_item
    return shuffled

def reorder_iter(seq, start):
    reorder = list(seq)
    return reorder[start:len(reorder)]+reorder[0:start]



def generate_users():
    global users_array
    global path

    # users
    # user0name...userNname
    # user0songs
    #       user0song0...user0songM
    #       user0artist0...user0artistM
    nsongs = 0
    nusers = 0
    try:
        users = open(path + 'gs_users', 'r+')

        for line in users:
            user_elements = line.split()
            print(line)
            users_array.append(user_elements[0])
            for song in client.collection(user_elements[0]):
    #           print(unicode(song.name).encode("utf-8"))
                rest_interface_put('user' + str(nusers) + 'song' + str(nsongs),  (strip_accents(unicode(song.name)).encode("utf-8")))
                rest_interface_put('user' + str(nusers) + 'artist' + str(nsongs),  (strip_accents(unicode(song.artist)).encode("utf-8")))
                nsongs+=1
            rest_interface_put('user' + str(nusers) + 'songs',  str(nsongs))
            rest_interface_put('user' + str(nusers) + 'name',  user_elements[1])
            nusers+=1

        users.close()
    except IOError:
        pass

    rest_interface_put('users',  str(nusers))


def generate_playlists():
    global playlist_array
    global path

    # playlists
    # playlist0name
    # playlist0songs
    #   playlist0song0...playlist0songM
    #   playlist0artist0...playlist0artistM
    nplaylists = 0
    nsongs = 0
    try:
        playlist = open(path + 'gs_playlists', 'r+')

        for line in playlist:
            playlist_array.append(line)
    #        print(client.playlist(line).name)
            for song in client.playlist(line).songs:
    #           print(unicode(song.name).encode("utf-8"))
                rest_interface_put('playlist' + str(nplaylists) + 'song' + str(nsongs), (strip_accents(unicode(song.name)).encode("utf-8")))
                rest_interface_put('playlist' + str(nplaylists) + 'artist' + str(nsongs), (strip_accents(unicode(song.artist)).encode("utf-8")))
                nsongs+=1
            rest_interface_put('playlist' + str(nplaylists) + 'songs', str(nsongs))
            rest_interface_put('playlist' + str(nplaylists) + 'name', (strip_accents(unicode(client.playlist(line).name)).encode("utf-8")))
            nplaylists+=1

        playlist.close()
    except IOError:
        pass

    rest_interface_put('playlists', str(nplaylists))


def generate_radios():
    global radios_array
    global path

    # radios
    # radio0name 
    nradios = 0
    radio = ""
    try:
        radios = open(path + 'gs_radios', 'r+')

        for line in radios:
            radio_element = line.split()
            radios_array.append(radio_element[0])
    #       print(line)
            rest_interface_put('radio' + str(nradios) + 'name', (strip_accents(unicode(radio_element[1])).encode("utf-8")))
            nradios+=1

        radios.close()
    except IOError:
        pass

    rest_interface_put('radios', str(nradios))







#main code
if linino:
    path = '/root/examples-pygrooveshark/'
    command_end = ''

while try_ping():
    time.sleep(1)

rest_interface_put('lininoStatus', 'Connecting to GroveShark')

client = Client()
client.init()

rest_interface_put('lininoStatus', 'Building User Info')

generate_users()

rest_interface_put('lininoStatus', 'Building Playlist')

generate_playlists()

rest_interface_put('lininoStatus', 'Building Radios ')

generate_radios()

current_pos = 0
song_duration = 0

rest_interface_put('lininoStatus', 'Starting Decoder')

# mpg123 Commands:
#   L: load file
#   P: pause playback
#   S: stop
#   V: volume (%)
mpg123_proc = subprocess.Popen(['mpg123', '-R'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)

rest_interface_put('command', 'none')
rest_interface_put('lininoStatus', 'Ready')

audio_iter = None
prev_pos = -1
element = 0

current_pos = 0
song_duration = 0

song = None
repeat = 0

while 1:
    command = rest_interface_get('command')
    if command != 'none':
        if command == 'playRadio'+command_end:
            radio = rest_interface_get('commandData1')
            audio_iter = client.radio(radios_array[int(radio)]).__iter__()
            stop_stream(mpg123_proc)

        elif command == 'playUser'+command_end:
            element = 0
            user = rest_interface_get('commandData1')
            user_song = rest_interface_get('commandData2')
            audio_iter = client.collection(users_array[int(user)])
            if shuffle:
                audio_iter = random_iter(audio_iter, int(user_song))
            else:
                audio_iter = reorder_iter(audio_iter, int(user_song))
            print("len       " + str(len(audio_iter)))
            stop_stream(mpg123_proc)

        elif command == 'playPlayList'+command_end:
            element = 0
            playlist = rest_interface_get('commandData1')
            playlist_song = rest_interface_get('commandData2')
            audio_iter = client.playlist(playlist_array[int(playlist)]).songs
            if shuffle:
                audio_iter = random_iter(audio_iter, int(playlist_song))
            else:
                audio_iter = reorder_iter(audio_iter, int(playlist_song))
            stop_stream(mpg123_proc)

        elif command == 'playPopular'+command_end:
            period = rest_interface_get('commandData1')
            if period == 'daily'+command_end:
                print('daily')
                audio_iter = client.popular(period=client.DAILY)
            else:
                audio_iter = client.popular(period=client.MONTHLY)
            stop_stream(mpg123_proc)

        elif command == 'shuffle'+command_end:
            shuffle_str = rest_interface_get('commandData1')
            if shuffle_str == 'enable'+command_end:
                shuffle = 1
            else:
                shuffle = 0
        
        elif command == 'volume'+command_end:
            vol = rest_interface_get('commandData1')
            set_volume(mpg123_proc, int(vol))

        elif command == 'pause'+command_end:
            pause_stream(mpg123_proc)
        
        elif command == 'next'+command_end:
            stop_stream(mpg123_proc)

        elif command == 'prev'+command_end:
            if type(audio_iter) == types.GeneratorType :
                repeat = 1
            else:
                if element > 1:
                    element-=2
                elif element > 0:
                    element -= 1
            stop_stream(mpg123_proc)

        elif command == 'restart'+command_end:
            if type(audio_iter) == types.GeneratorType :
                repeat = 1
            else:
                if element > 0:
                    element-=1
            stop_stream(mpg123_proc)

        elif command == 'addUser'+command_end:
            param_id = rest_interface_get('commandData1')
            param_name = rest_interface_get('commandData2')
            try:
                users = open('gs_users', 'a')
                
                users_param = param_id + " " + param_name

                users.write(users_param.replace('\n', ''))

                users.close()
            except IOError:
                pass

            users_array = []
            generate_users()

        elif command == 'addRadio'+command_end:
            param_id = rest_interface_get('commandData1')
            param_name = rest_interface_get('commandData2')
            try:
                radios = open('gs_radios', 'a')
                
                radios_param = param_id + " " + param_name

                radios.write(radios_param.replace('\n', ''))

                radios.close()
            except IOError:
                pass

            radios_array = []
            generate_radios()


        elif command == 'addPlaylist'+command_end:
            param_id = rest_interface_get('commandData1')
            try:
                playlists = open('gs_radios', 'a')
                
                playlists_param = param_id

                playlists.write(playlists_param.replace('\n', ''))

                playlists.close()
            except IOError:
                pass

            playlists_array = []
            generate_playlists()

    readable = select.select([mpg123_proc.stdout], [], [], 0.25)[0]
    for s in readable:
        mpg123_line = mpg123_proc.stdout.readline() 
#        print(mpg123_line)
        eos = mpg123_line.find('@P 0')
        pos = mpg123_line.find('@F')
        
        if (pos != -1) :
            current_pos = int(float(mpg123_line.split()[3]))
            if song.duration == 0 or song.duration == None:
                song_duration = current_pos + int(float(mpg123_line.split()[4]))
            else:
                song_duration = song.duration                    
#                print(current_pos + " - " + str(song.duration))

        if (eos != -1) :
            if audio_iter != None:
                try:
                    if type(audio_iter) == types.GeneratorType :
                        if repeat:
                            repeat = 0
                        else:
                            song = audio_iter.next()
                    else:
                        print("element   " + str(element))
                        print("len       " + str(len(audio_iter)))
                        song = audio_iter[element]
                    load_stream(mpg123_proc, song)
                    prev_pos = -1
                    current_pos = 0
                    song_duration = 0
                    element += 1
                except StopIteration:
                    print("End of playback")
            break
# This is to disable playback, so that we don't have to wait for the full audio to play
#        stop_stream(mpg123_proc)
    if prev_pos != current_pos :   
#            print(str(current_pos) + " - " + str(song_duration))
        rest_interface_put('nowPlaying_pos', str(current_pos))
        rest_interface_put('nowPlaying_dur', str(song_duration))
        prev_pos = current_pos



