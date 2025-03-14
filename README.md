# GibberLink official repo

> [!Caution]
> Scam projects circulate around impersonating this project and its creators. We don't sell anything including crypto, webinars, etc. This readme is the source of truth.

## Demo
[gbrl.ai](https://www.gbrl.ai/) — Agent2Agent conversation in your browser (use two devices)

[youtube](https://www.youtube.com/watch?v=EtNagNezo8w) — Agents switching from english to ggwave, video:

[![Agents switching from english to ggwave video](https://img.youtube.com/vi/EtNagNezo8w/maxresdefault.jpg)](https://www.youtube.com/watch?v=EtNagNezo8w)

## Authors

Contact us: contact@gbrl.ai

Anton Pidkuiko: [threads](https://www.threads.net/@anton10xr), [linkedin](https://www.linkedin.com/in/anton-pidkuiko-7535409b), [github](https://github.com/anton10xr)

Boris Starkov: [linkedin](https://www.linkedin.com/in/boris-starkov/), [github](https://github.com/PennyroyalTea)

based on [ggwave](https://github.com/ggerganov/ggwave) library by [Georgi Gerganov](https://github.com/ggerganov) and conversational AI by [ElevenLabs](https://try.elevenlabs.io/gibberlink)

## How it works
* Two independent conversational [ElevenLabs](https://try.elevenlabs.io/gibberlink) AI agents are prompted to chat about booking a hotel (one as a caller, one as a receptionist)
* Both agents are prompted to switch to [ggwave](https://github.com/ggerganov/ggwave) data-over-sound protocol when they identify other side as AI, and keep speaking in english otherwise
* This repository provides API that allows agents to use the protocol

Bonus: you can open the [ggwave web demo](https://waver.ggerganov.com/), play the video above and see all the messages decoded!

## How to repro
https://github.com/PennyroyalTea/gibberlink/wiki/Repro-steps-for-demo
