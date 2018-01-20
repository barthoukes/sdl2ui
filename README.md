sdl2ui features
===============
  SDL2UI can create window GUI applications with UI, buttons, mouse, touch, RFID etc.
  SDL2UI is based on SDL1.2 and later SDL2.0

  The graphical renderer from SDL is used to show output on the display
  Images are reponsive when you touch them. A dark background circle when you press them, which decreases fast when released.
  No use of a big SDL_Surface, but each button is a seperate SDL_Surface, more flexible and easier to change to SDL2.0
  2D matrix of buttons is possible with soft scroll in verticle or horizontal direction.
  After glow for each button, image, background in different shapes.
  Play audio wave forms automatic
  Background for several objects with rounded corners for any radius
  Backgrounds can have single colour
  Backgrounds can have a vertical change gradually from one to another colour
  Backgrounds may have a pyramid change gradually from one to another colour
  Backgrounds may be split in 2 colours, top half and bottom half have another colour
  Backgrounds may cut a piece from a background picture, which requires a background image of your screen resolution.
  Bar graphs are easy to display bar charts
  Button response. Buttons become dark when you press them and slowly become light again when released.
  Buttons with text and image may be created and displayed
  Buttons have a border which is adjustable in width
  Buttons may have a text inside
  Buttons need to align the picture on 8 positions around the text
  Text may be in all colours only 1 colour
  Text can be in any font, font size for arabian and other font and other font size for chinese/japanese.
  Text can have filtering for display or no filtering for printing
  Text can be in a bitmap to do other things than display (e.g. print)
  Text can have a shade
  Buttons can be positioned every 8 pixels and have a width with a multiple of 8 pixels
  Buttons can have a margin between buttons of any number of pixels
  Button text can have a margin of any number of pixels from the edge of a button
  Buttons can be moved to any location
  Buttons can be visibile or invisible
  Buttons may be dragged around the screen if enabled
  Buttons can be in many languages, with an external accessor for languages and text
  Buttons choose the optimal picture size with several pictures on the disk
  Buttons have a background with all posibilities of a background
  CtextButton is derived from button to show text
  CmenuButton is derived from button to show a menu item
  CbottomButton is derived from button to show a button on the bottom of the screen
  CkeyboardButton is a derived from button to show a keyboard
  CimageButton is derived from button to show an image inside the button
  CheaderButton is derived from button to show a header title on each screen
  CmatrixButton is derived from button and adjusted for 1D and 2D scrolling
  CgraphButton is derived from button to show a graph
  CiconButton is derived from button to show an icon
  The handwriter can be used to input Chinese Japanese and others
  The handwriter is derived from the open source project Zinnia
  The handwriter works with Simplified and Original Chinese
  The handwriter shows the 50 most look-a-like symbols in a swipe dialog
  The handwriter has thick lines to show
  The handwriter can be cleaned externally to start a new symbol
  The handwriter can have any size on the screen
  The handwriter paints itself for updates on screen
  The handwriter searches for symbols every time the finger releases the touchscreen

To be continued
===============
The archive is not filled yet with anything. I'm very busy, but want to make this project work... so be a little patience with me.

Soon here will be following objects:
- Buttons : Edit buttons, Text buttons, Keyboard buttons, Bottom buttons, Image buttons
- Images : With backgrounds, rounded rectangles, adjustable gravity, labels
- Sliders
- Swype dialog going up/down or left/right
- 2D swype dialog (matrix)
- Drag and drop, also from swype list
- Dialog solution
- Messagebox solution
- Edit text
- Chinese input method (no, I am not a Chinese)

Build
=====
Use Eclipse Oxygen to build all
First install following for debian distributions:
sudo apt-get install libzinnia-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev g++

