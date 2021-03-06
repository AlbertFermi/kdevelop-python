#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
""":synopsis: Dumb wrapper around threading.

:mod:`multiprocessing.dummy` replicates the API of :mod:`multiprocessing` but is
no more than a wrapper around the :mod:`threading` module.


.. rogramming guidelines
----------------------

There are certain guidelines and idioms which should be adhered to when using
:mod:`multiprocessing`.


All platforms
~~~~~~~~~~~~~

Avoid shared state

As far as possible one should try to avoid shifting large amounts of data
between processes.

It is probably best to stick to using queues or pipes for communication
between processes rather than using the lower level synchronization
primitives from the :mod:`threading` module.

Picklability

Ensure that the arguments to the methods of proxies are picklable.

Thread safety of proxies

Do not use a proxy object from more than one thread unless you protect it
with a lock.

(There is never a problem with different processes using the *same* proxy.)

Joining zombie processes

On Unix when a process finishes but has not been joined it becomes a zombie.
There should never be very many because each time a new process starts (or
:func:`active_children` is called) all completed processes which have not
yet been joined will be joined.  Also calling a finished process's
:meth:`Process.is_alive` will join the process.  Even so it is probably good
practice to explicitly join all the processes that you start.

Better to inherit than pickle/unpickle

On Windows many types from :mod:`multiprocessing` need to be picklable so
that child processes can use them.  However, one should generally avoid
sending shared objects to other processes using pipes or queues.  Instead
you should arrange the program so that a process which need access to a
shared resource created elsewhere can inherit it from an ancestor process.

Avoid terminating processes

Using the :meth:`Process.terminate` method to stop a process is liable to
cause any shared resources (such as locks, semaphores, pipes and queues)
currently being used by the process to become broken or unavailable to other
processes.

Therefore it is probably best to only consider using
:meth:`Process.terminate` on processes which never use any shared resources.

Joining processes that use queues

Bear in mind that a process that has put items in a queue will wait before
terminating until all the buffered items are fed by the "feeder" thread to
the underlying pipe.  (The child process can call the
:meth:`Queue.cancel_join_thread` method of the queue to avoid this behaviour.)

This means that whenever you use a queue you need to make sure that all
items which have been put on the queue will eventually be removed before the
process is joined.  Otherwise you cannot be sure that processes which have
put items on the queue will terminate.  Remember also that non-daemonic
processes will be automatically be joined.

An example which will deadlock is the following::

from multiprocessing import Process, Queue

def f(q):
q.put('X' * 1000000)

if __name__ == '__main__':
queue = Queue()
p = Process(target=f, args=(queue,))
p.start()
p.join()                    # this deadlocks
obj = queue.get()

A fix here would be to swap the last two lines round (or simply remove the
``p.join()`` line).

Explicitly pass resources to child processes

On Unix a child process can make use of a shared resource created in a
parent process using a global resource.  However, it is better to pass the
object as an argument to the constructor for the child process.

Apart from making the code (potentially) compatible with Windows this also
ensures that as long as the child process is still alive the object will not
be garbage collected in the parent process.  This might be important if some
resource is freed when the object is garbage collected in the parent
process.

So for instance ::

from multiprocessing import Process, Lock

def f():
more do something using "lock" more

if __name__ == '__main__':
lock = Lock()
for i in range(10):
Process(target=f).start()

should be rewritten as ::

from multiprocessing import Process, Lock

def f(l):
more do something using "l" more

if __name__ == '__main__':
lock = Lock()
for i in range(10):
Process(target=f, args=(lock,)).start()

Beware replacing sys.stdin with a "file like object"

:mod:`multiprocessing` originally unconditionally called::

os.close(sys.stdin.fileno())

in the :meth:`multiprocessing.Process._bootstrap` method --- this resulted
in issues with processes-in-processes. This has been changed to::

sys.stdin.close()
sys.stdin = open(os.devnull)

Which solves the fundamental issue of processes colliding with each other
resulting in a bad file descriptor error, but introduces a potential danger
to applications which replace :func:`sys.stdin` with a "file-like object"
with output buffering.  This danger is that if multiple processes call
:func:`close()` on this file-like object, it could result in the same
data being flushed to the object multiple times, resulting in corruption.

If you write a file-like object and implement your own caching, you can
make it fork-safe by storing the pid whenever you append to the cache,
and discarding the cache when the pid changes. For example::

@property
def cache(self):
pid = os.getpid()
if pid != self._pid:
self._pid = pid
self._cache = []
return self._cache

For more information, see :issue:`5155`, :issue:`5313` and :issue:`5331`

Windows
~~~~~~~

Since Windows lacks :func:`os.fork` it has a few extra restrictions:

More picklability

Ensure that all arguments to :meth:`Process.__init__` are picklable.  This
means, in particular, that bound or unbound methods cannot be used directly
as the ``target`` argument on Windows --- just define a function and use
that instead.

Also, if you subclass :class:`Process` then make sure that instances will be
picklable when the :meth:`Process.start` method is called.

Global variables

Bear in mind that if code run in a child process tries to access a global
variable, then the value it sees (if any) may not be the same as the value
in the parent process at the time that :meth:`Process.start` was called.

However, global variables which are just module level constants cause no
problems.

Safe importing of main module

Make sure that the main module can be safely imported by a new Python
interpreter without causing unintended side effects (such a starting a new
process).

For example, under Windows running the following module would fail with a
:exc:`RuntimeError`::

from multiprocessing import Process

def foo():
print 'hello'

p = Process(target=foo)
p.start()

Instead one should protect the "entry point" of the program by using ``if
__name__ == '__main__':`` as follows::

from multiprocessing import Process, freeze_support

def foo():
print 'hello'

if __name__ == '__main__':
freeze_support()
p = Process(target=foo)
p.start()

(The ``freeze_support()`` line can be omitted if the program will be run
normally instead of frozen.)

This allows the newly spawned Python interpreter to safely import the module
and then run the module's ``foo()`` function.

Similar restrictions apply if a pool or manager is created in the main
module.


.. xamples
--------

Demonstration of how to create and use customized managers and proxies:

"""
