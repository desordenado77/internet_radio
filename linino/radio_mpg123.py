# -*- coding:utf-8 -*-
#
# Copyright (C) 2012, Maximilian Köhl <linuxmaxi@googlemail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.



from __future__ import print_function
import urllib
import urllib2
import subprocess
import sys

from grooveshark import Client
from grooveshark.classes import Radio

client = Client()
client.init()
cmdargs = str(sys.argv)

url_name='http://localhost/data/put/songName/'
url_artist='http://localhost/data/put/songArtist/'
url_album='http://localhost/data/put/songAlbum/'

for song in client.radio(sys.argv[1]):
    
    song_name = unicode(song.name).encode("utf-8")
    song_artist_name = unicode(song.artist.name).encode("utf-8")
    song_album_name = unicode(song.album.name).encode("utf-8")


    print(song_name)
    print(song_artist_name)
    print(song_album_name)

    url_name_curl = url_name + urllib.quote(song_name)
#    print(url_name_curl)
#    subprocess.call(['curl', url_name_curl])
    urllib2.urlopen(url_name_curl)
    url_artist_curl = url_artist + urllib.quote(song_artist_name)
#    subprocess.call(['curl', url_artist_curl])
    urllib2.urlopen(url_artist_curl)
    url_album_curl = url_album + urllib.quote(song_album_name)
#    subprocess.call(['curl', url_album_curl])
    urllib2.urlopen(url_album_curl)

    sys.stdout.flush()
    subprocess.call(['mpg123', song.stream.url])
