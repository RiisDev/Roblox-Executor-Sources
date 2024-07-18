# Roblox-Integrity-Check-Bypass

# Credits
* [gogo1000](https://github.com/gogo9211)
* [iivillian](https://github.com/iivillian)
* [0x90](https://github.com/AmJayden)

# How to use
Include the utils.hpp and memcheck.hpp file within your dll. Call memcheck::initiate() once before editing any memory.

# How it works
This bypass works by collecting the unseeded hashes of every chunk. The main hasher and active silent hasher are then hooked and the hashes of the chunks within are spoofed with the clean hash calculated against the seed that the server sent.

# Reversal
Roblox integrity check is made of 2 core parts. They have one 'core' hasher and 16 separate hashers. Only one of these 16 hashers are active (which is decided per server) along with the 'core' hasher.
For this writeup I will call the core hasher 'main' hasher and rest I will just call hashers

![](https://cdn.discordapp.com/attachments/782842266456817664/956289832290758656/gkp0qufp.png)
![](https://cdn.discordapp.com/attachments/782842266456817664/956290152928526356/jBCw6PAt.png)

The first picture is an example of 1 of the 16 hashers and the second picture is of the main hasher. Every single one of those 16 hashers create a unique hash so you can't just patch one and redirect rest to it, making bypassing it more difficult.

Only one of them will be active per server and it will be same for everyone in it.
The main hasher is active all the time.

![](https://cdn.discordapp.com/attachments/782842266456817664/956291857787256902/ZAy2wggF.png)

The following function is responsible for calling the active hasher, the arguments passed follows: chunk_start, chunk_size, unknown, an unique value we named seed.
Once the active hasher finishes hashing all of the chunks (30 as of now), the seed will change. This prevents you from simply storing the returned hash and reusing it.

So what we do for those hashers is call the active one ourselfs with last argument(seed) being 0, which will give us the unique hash and then we save it.
For that we need to get the chunks and their sizes.

```
const auto chunk_o = chunk_start[i];
const auto chunk = m_base - 0x400000 + ((0x1594FE2C * chunk_o - 0x32E7FDBF) ^ (0xD2C21B15 - 0x344B5409 * chunk_o)) + 2;

const auto size_o = size_start[i];
const auto size = (0x1594FE2C * size_o - 0x32E7FDBF) ^ (0xD2C21B15 - 0x344B5409 * size_o);
```

Now all hashers do different operation with this seed so we have function which returns what operation the active hasher is using.
Then all we have to do is hook the hasher and return the result between the hash and the seed using the operation we determined earlier.

Now for the main hasher things are a bit more complicated.

![](https://cdn.discordapp.com/attachments/782842266456817664/956295870444363796/uETIQD1K.png)

One chunk is hashed per call to the main hasher, this hash is then xored by the seed and added to the container shown in the picture above.
The active seed changes after every chunk is hashed (30 chunks as of now)
The function does more math with the seed before xoring so we can't use same technique as we do on hashers.

What we did here is set seed to 0, called the function 30 times ( until it hashed every chunk ) and restored the old seed ( so when Roblox call the function themselfs it won't get invalid hash )

If seed is 0 the value it xors by is always 0x68178A72. After they do their math so we can simply do the inverse operation to retrieve a fully unseeded hash of the current chunk and then we store the core hashes too.

Then we just hook the main and active hasher.
