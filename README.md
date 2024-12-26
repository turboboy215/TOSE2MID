# TOSE2MID
TOSE (GB/GBC) to MIDI converter

This tool converts music (and sound effects) from Game Boy and Game Boy Color games using TOSE's sound engine to MIDI format. This has actually been found to be the most widely used sound engine for both GB and GBC games combined.
Due to complications with how the pointers for sequences are stored, a special .cfg configuration file needs to be used with each game specifying where to look for the song data in the game. Configuration files for every game, including multiple variants for some games with different offsets per region, are also included with the program. These configuration files were created manually, and may not always be 100% perfect. Note that for many games, there are "empty" tracks. This is normal.
Support for the NES version of the driver is at least partially supported; however, this has only been confirmed to work with Dragon Ball.

Examples:
* MC2MID "Yoshi's Cookie (U) [!].gb" "Yoshi's Cookie (U) [!].cfg"
* MC2MID "Kirby's Block Ball (U) [S][!].gb" "Kirby's Block Ball (U) [S][!].cfg"
* MC2MID "Harvest Moon GB (E) [C][!].gbc" "Harvest Moon GB (E) [C][!].cfg"

This tool was based on my own reverse-engineering of the sound engine. As usual, a "prototype" converter, TOSE2TXT, is also included.

Supported games:
  * Bad Badtz-Maru Robo Battle
  * Bakuchou Retrieve Master
  * Bases Loaded
  * Battle Unit Zeoth
  * Bikkuriman 2000: Charging Card GB
  * Blodia
  * Command Master
  * Crayon Shin-chan
  * Crayon Shin-chan: Ora no Gokigen Collection
  * Crayon Shin-chan 2: Ora to Wanpaku Gokko Dazo
  * Crayon Shin-chan 3: Ora no Gokigen Athletic
  * Crayon Shin-chan 4: Ora no Itazura Daihenshin (also Super Mario Land 4)
  * Dancing Furby
  * Dear Daniel no Sweet Adventure: Kitty-chan o Sagashite
  * Dodge Boy
  * Dragon Warrior I & II
  * Dragon Warrior III
  * Dragon Warrior Monsters
  * Dragon Warrior Monsters 2: Cobi's Journey
  * Dragon Warrior Monsters 2: Tara's Adventure
  * Elie no Atelier GB
  * Fairy Kitty no Kaiun Jiten: Yousei no Kuni no Uranai Shugyou
  * Football International
  * Game & Watch Gallery
  * Game & Watch Gallery 2
  * Game & Watch Gallery 3
  * Ganbare Goemon: Tengutou no Gyakushuu
  * GB Harobots
  * Goal!
  * Gurander Musashi RV
  * Harvest Moon 2 GBC
  * Harvest Moon 3 GBC
  * Harvest Moon GB
  * Hatris
  * Heavyweight Championship Boxing
  * Hello Kitty no Beads Koubou
  * Hello Kitty no Sweet Adventure: Daniel-kun ni Aitai
  * Helly Kitty to Dear Daniel no Dream Adventure
  * Hero Hero Kun
  * Hyper Lode Runner
  * International Track & Field: Summer Games
  * Kid Icarus: Of Myths and Monsters
  * Kirby's Block Ball
  * Kisekae Series 2: Oshare Nikki
  * Konami GB Collection: Vol. 1*
  * Konami GB Collection: Vol. 2*
  * Konami GB Collection: Vol. 3*
  * Konami GB Collection: Vol. 4*
  * Legend of the River King 2
  * Legend of the River King GB
  * Legend of the Sea King GB
  * Lodoss Tou Senki: Eiyuu Kishiden GB
  * Mach Go Go Go
  * Magical Talutokun: Raiba Zone Panic
  * Majokko Mari-chan no Kisekae Monogatari
  * Malibu Beach Volleyball
  * Marie no Atelier GB
  * Mario Tennis
  * Maru's Mission
  * Masakari Densetsu Kintarou: Action Game Hen
  * Master Karateka
  * Medarot Cardrobottle: Kabuto Version
  * Medarot Cardrobottle: Kuwagata Version
  * Metal Gear Solid
  * Metal Walker
  * Midori no Makibaoo
  * Musashi Road
  * Namco Classics
  * Namco Gallery Vol. 1*
  * Namco Gallery Vol. 2*
  * Namco Gallery Vol. 3*
  * Ninku
  * Ninku Dai 2: Ninku Sensou Hen
  * Nushi Tsuri Adventure: Kite no Bouken
  * Otoko Jyuku
  * Pia Carrot he Youkoso!! 2.2
  * Pipe Dream
  * Play Action Football
  * Pocket Sonar
  * Roadster
  * Sakura Taisen GB: Geki Hana Kumi Nyuutai!
  * Sanrio Timenet: Kako Hen
  * Sanrio Timenet: Mirai Hen
  * Space-Net: Cosmo Blue
  * Space-Net: Cosmo Neo
  * Space-Net: Cosmo Red
  * Sports Collection
  * Super Bombliss DX
  * Tales of Phantasia: Narikiri Dungeon
  * Tamagotchi 3
  * TDL (Tokyo Disneyland) Fantasy Tour
  * Tennis
  * Tetris 2
  * Tetris Blast
  * The Tower of Druaga
  * Tsuriiko!!
  * Ultraman Chou Toushi Gekiden
  * Ultraman Club
  * Winnie the Pooh: Adventures in the 100 Acre Wood
  * World Ice Hockey
  * Yoshi's Cookie
  * Yuu Yuu Hakusho
  * Yuu Yuu Hakusho: Ankoku Bujutsu Kai
  * Yuu Yuu Hakusho: Makai no Tobira
  * Yuu Yuu Hakusho: Makai Touitsu

## To do:
  * Panning support
  * Complete support for the NES version of the sound engine and add WonderSwan support. (Both versions appear to be almost the same as the Game Boy version)
  * GBS file support
