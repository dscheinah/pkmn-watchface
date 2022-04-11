# Pokémon Gamified Watchface

This is a fan art inspired by Pokémon.

The watch uses the battlefield to render date, time and various health statistics:
- Enemy level increases with sleep time.
- Own level increases with steps.
- Experience reaches 100% if you walked more than the day before.
- Own health decreases with remaining battery.
- Enemy health is reduced every hour based on level difference, types and your time spent active.

Also there are certain events you can explore that change the selected monsters.

It is also possible to skip parts of the hourly events when another screen is active. This may or may not be an advantage.

All assets have been taken from the original game and are subject to Pokémon and Nintendo/Creatures Inc./GAME FREAK.

# App-Store

http://apps.rebble.io/de_DE/application/620187963ea70300099815fa

# Development

Use the `bootstrap.sh` to download and prepare the resources. This script uses `curl` and `tar` to download the assets from https://veekun.com/dex/downloads. For aplite support also `convert` from ImageMagick and maybe `optipng` are needed.

The `docker-compose.yml` provides a setup for the SDK by configuring https://hub.docker.com/r/bboehmke/pebble-dev. Use it with:
`docker-compose run --rm sdk`.

If you are not able to use Docker (e.g. the emulator does not work on Windows), you can download a prebuilt VM disk for https://www.virtualbox.org/ from https://willow.systems/pebble/vm/.

You can compile the app in test mode with `pebble build -- --test`.
