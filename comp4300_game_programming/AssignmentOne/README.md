# Assignment One - Geometry Bouncing

## Config File

### Window W H

This line declares that the SFML Window must be constructed.

```txt
Window Width    W   undigned
Window Height   H   undigned
```

### Font F S R G B

This lines defines the font which is to be used to draw text for this program.
The format of the line is as follows:

```txt
Font File       F           std::string
Font Size       S           undigned
RGB Color       R, G, B     uint8_t, uint8_t, uint8_t
```

### Rectangle N X Y SX SY R G B W H

Defines a RectangleShape with:

```txt
Shape Name          N           std::string
Initial Position    X, Y        float, float
Initial Speed       SX, SY      float, float
RGB Color           R, G, B     uint8_t, uint8_t, uint8_t
Size                W, H        float, float
```


### Circle N X Y SX SY R G B R

Defines a CircleShape with:

```txt
Shape Name          N           std::string
Initial Position    X, Y        float, float
Initial Speed       SX, SY      float, float
RGB Color           R, G, B     uint8_t, uint8_t, uint8_t
Size(Radius)        S           float
```
