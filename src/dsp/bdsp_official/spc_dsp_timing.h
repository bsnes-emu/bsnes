// Execute clock for a particular voice
#define V( clock, voice )   voice_##clock( &m_voice_state [voice] );

/* The most common sequence of clocks uses composite operations
for efficiency. For example, the following are equivalent to the
individual steps on the right:

V(2_31  ,2) -> V( 2,2) V(31,3)
V(3_0_29,2) -> V( 3,2) V( 0,3) V(29,4)
V(4_1_30,2) -> V( 4,2) V( 1,3) V(30,4) */

// Voice      0      1      2      3      4      5      6      7
PHASE( 0)  V(V5,0)V(V2,1)
PHASE( 1)  V(V6,0)V(V3,1)
PHASE( 2)  V(V7_V4_V1,0)
PHASE( 3)  V(V8_V5_V2,0)
PHASE( 4)  V(V9_V6_V3,0)
PHASE( 5)         V(V7_V4_V1,1)
PHASE( 6)         V(V8_V5_V2,1)
PHASE( 7)         V(V9_V6_V3,1)
PHASE( 8)                V(V7_V4_V1,2)
PHASE( 9)                V(V8_V5_V2,2)
PHASE(10)                V(V9_V6_V3,2)
PHASE(11)                       V(V7_V4_V1,3)
PHASE(12)                       V(V8_V5_V2,3)
PHASE(13)                       V(V9_V6_V3,3)
PHASE(14)                              V(V7_V4_V1,4)
PHASE(15)                              V(V8_V5_V2,4)
PHASE(16)                              V(V9_V6_V3,4)
PHASE(17)  V(V1,0)                            V(V7,5)V(V4,6)
PHASE(18)                                     V(V8_V5_V2,5)
PHASE(19)                                     V(V9_V6_V3,5)
PHASE(20)         V(V1,1)                            V(V7,6)V(V4,7)
PHASE(21)                                            V(V8,6)V(V5,7)  V(V2,0) // t_brr_next_addr order dependency
PHASE(22)  V(V3a,0)                                  V(V9,6)V(V6,7)  echo_22();
PHASE(23)                                                   V(V7,7)  echo_23();
PHASE(24)                                                   V(V8,7)  echo_24();
PHASE(25)  V(V3b,0)                                         V(V9,7)  echo_25();
PHASE(26)                                                            echo_26();
PHASE(27) misc_27();                                                 echo_27();
PHASE(28) misc_28();                                                 echo_28();
PHASE(29) misc_29();                                                 echo_29();
PHASE(30) misc_30();V(V3c,0)                                         echo_30();
PHASE(31)  V(V4,0)       V(V1,2)

#undef V
