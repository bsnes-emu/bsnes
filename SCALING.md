# Scaling

Starting with version 0.4, the Cocoa version of SameBoy supports several GPU-accelerated scaling algorithms, some of which made their premiere at SameBoy. This document describes the algorithms supported by SameBoy.

## General-purpose Scaling Algorithms
Common algorithms that were not made specifically for pixel art

### Nearest Neighbor
A simple pixelated scaling algorithm we all know and love. This is the default filter.

### Bilinear
An algorithm that fills "missing" pixels using a bilinear interpolation, causing a blurry image

### Smooth Bilinear
A variant of bilinear filtering that applies a smooth curve to the bilinear interpolation. The results look similar to the algorithm Apple uses when scaling non-Retina graphics for Retina Displays.

## The ScaleNx Family
The ScaleNx family is a group of algorithm that scales pixel art by the specified factor using simple pattern-based rules. The Scale3x algorithm is not yet supported in SameBoy.

### Scale2x
The most simple algorithm of the family. It scales the image by a 2x factor without introducing new colors.

### Scale4x
This algorithm applies the Scale2x algorithm twice to scale the image by a 4x factor.

### Anti-aliased Scale2x
A variant of Scale2x exclusive to SameBoy that blends the Scale2x output with the Nearest Neighbor output. The specific traits of Scale2x makes this blend produce nicely looking anti-aliased output.

### Anti-aliased Scale4x
Another exclusive algorithm that works by applying the Anti-aliased Scale2x algorithm twice

## The HQnx Family
A relatively modern family of scaling algorithms that makes an extensive use of lookup tables to create scaled anti-aliased output. The HQnx family includes several scaling factors and variants.

### HQ2x
Currently HQ2x is the only HQnx algorithm in SameBoy. As the name implies, it scales the image by a factor of 2.

### The OmniScale algorithm
OmniScale is an exclusive algorithm developed for SameBoy. It is inspired by HQnx's lookup tables, but improves on them by handling more cases. OmniScale can scale an image by any factor, including non-integer factors, and produces high quality anti-aliased output.

## The OmniScale Legacy Family (beta)
An old prototype of the OmniScale algorithm. It combines pattern-based rules with a unique locally paletted bilinear filtering technique to scale an image by any factor, including non-integer factors. Its pattern-based rule do not currently detect 30- and 60-degree diagonals, making them look jaggy. The output OmniScale Legacy produces is quite unique, as it tends to produce non-trivial patterns.

### OmniScale Legacy
The base version of the algorithm, which generates aliased output with very few new colors introduced.

### Anti-aliased OmniScale Legacy
A variant of OmniScale that produces anti-aliased output using 2x super-sampling.