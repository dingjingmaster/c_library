#!/usr/bin/env python
# -*- coding=utf-8 -*-
import os
import pyquery
import requests
import threading

header = {
        'Accept'            : 'application/json, text/javascript, */*; q=0.01',
        'Cache-Control'     : 'no-cache', 
        'Connection'        : 'close',
        }

class ThreadPool:
    _stop = False
    _thread = []
    _spider = []
    _thread_count = 10
    _mutex = threading.Lock()

    def working (self, thread_id: int):
        i = 0
        while True:
            s = requests.Session()
            s.headers = header
            r = s.get('https://zhile.io/2020/11/18/jetbrains-eval-reset-deprecated.html', timeout=10, allow_redirects=True)
            if r.status_code == requests.codes.ok:
                i += 1
                print ("%s \n" % (r.text))
                print ("%d" % i)

    def run (self):
        for i in range (0, 1000):
            t = ThreadPool.Thread(self.working, i + 1)
            self._thread.append (t)
        for i in self._thread:
            i.start()
        for i in self._thread:
            i.join()


    class Thread(threading.Thread):
        def __init__ (self, route, tid: int):
            threading.Thread.__init__(self)
            self._tid = tid
            self._route = route

        def run (self):
            self._route(self._tid)



if __name__ == '__main__':
    #del os.environ['ALL_PROXY']
    #del os.environ['all_proxy']

    p = ThreadPool()
    p.run()
