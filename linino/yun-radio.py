
from __future__ import print_function
import urllib
import urllib2
import subprocess
import sys

from grooveshark import Client
from grooveshark.classes import Radio


def rest_interface(url):
#    urllib2.urlopen(url)
    print(url)


url_name='http://localhost/data/put/'

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
    print(line)
    for song in client.collection(line):
#        print(unicode(song.name).encode("utf-8"))
        rest_interface(url_name + 'user' + str(nusers) + 'song' + str(nsongs) + '/' + urllib.quote(unicode(song.name).encode("utf-8")))
        nsongs+=1
    rest_interface(url_name + 'user' + str(nusers) + 'songs/' + str(nsongs))
    rest_interface(url_name + 'user' + str(nusers) + 'name/' + line)
    nusers+=1

rest_interface(url_name + 'users/' + str(nusers))

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
        rest_interface(url_name + 'playlist' + str(nplaylists) + 'song' + str(nsongs) + '/' + urllib.quote(unicode(song.name).encode("utf-8")))
        nsongs+=1
    rest_interface(url_name + 'playlist' + str(nplaylists) + 'songs/' + str(nsongs))
    rest_interface(url_name + 'playlist' + str(nplaylists) + 'name/' + urllib.quote(unicode(client.playlist(line).name).encode("utf-8")))
    nplaylists+=1

rest_interface(url_name + 'playlists/' + str(nplaylists))

#
# radios
# radio0name 
nradios = 0
for line in radios:
#    print(line)
    rest_interface(url_name + 'radio' + str(nradios) + 'name/' + urllib.quote(unicode(line).encode("utf-8")))
    nradios+=1

rest_interface(url_name + 'radios/' + str(nradios))




