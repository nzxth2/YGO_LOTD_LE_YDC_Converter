# YGO LOTD Link Evolution YDC Converter
 Converter for .ydc deck files of Yu-Gi-Oh! Legacy of the Duelist Link Evolution 

## Features
 - Converts .ydc deck file to humanly readable .txt files
 - Converts said .txt files back to .ydc files
 - Allows for modification of existing decks or addition of new decks
 - Deck size check is optional, disable at your own risk
 - If a card in a .txt file can not be identified, it will be recorded in missingCards.txt
  
 ### TXT File Format
 ```
 # main deck. first the card count, then the list
 40
 CardName1
 .
 .
 .
 CardName40
 # extra deck. first the card count, then the list
 15
 CardName1
 .
 .
 .
 CardName15
 # side deck. first the card count, then the list
 0
 # unknown value. leave it as it is or set it to 0. alternatively just omit the line
 25740
 ```
  
 Note that comments start with `#` and are always ignored, as are empty lines
 
## Building
  Not crossplatform at all, only builds for Windows  
  Included the makefile used with gcc, a simple `make` should do the trick  
  No external libraries should be required

## Credits
 Developed by nzxth2  
 [Nuklear GUI library](https://github.com/Immediate-Mode-UI/Nuklear) developed by Micha Mettke and others  
 [Notes](https://github.com/MoonlitDeath/Legacy-of-the-Duelist-notes/wiki) and [guide](https://github.com/MoonlitDeath/Link-Evolution-Editing-Guide/wiki) by MoonlitDeath

## Gallery
 ![Screenshot](https://i.imgur.com/TSchiAe.png)