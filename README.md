# circ

A simple IRC bot in C. Built as an itch-to-scratch project.

It's currently in active use, but in a small, trusted behaviour channel. It's still alpha, and best thought of as being in the prototyping stage. It currently supports joining a single channel, and four users commands:

* `.op` to +o either the sender of the requested user
* `.topic` to set the channel topic.
* `.time` to announce the time in PST/PDT (Vancouver), EST/EDT (New York), and GMT/BST (London).
* `.quit` to disconnect the bot.

## Usage
* Compile with a simple `make`.
* Edit file `users`. This must have at least one line containing the hostmask of the owning user, after which additional user hostmasks can be added. There is a hard limit of 32 users at the moment, but this will be lifted.
* Run with `./bot <alias> <server> <port> <channel>`.
