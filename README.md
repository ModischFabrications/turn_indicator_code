# turn_indicator_code

This is the Arduino code for [Turn Indicator](https://github.com/ModischFabrications/Turn_Indicator).

## Code
"A Class with one instance is like a group with one person. " -> Use a namespace instead
classes won't allow pointers and make anonymous function usage difficult

## References

### Libraries
Button debounce & more was initially managed manually as with all previous libraries, 
but I'm now old enough to trust other peoples work. 

- https://github.com/JChristensen/JC_Button : Seems nice, but I need events more than states
- https://github.com/mathertel/OneButton : nice events, simple presets, examples, active development; all that you can wish for
- https://github.com/bxparks/AceButton : like OneButton, but more explanations, tests and sensible features. Also MIT >> all. dislike generic handler for simple scripts
- ... too small for credibility & sustainability

OneButton was the winner in this case. 
