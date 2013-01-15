string SuperFamicom = R"(

database revision=2013-01-14

release
  cartridge region=NTSC
    board id:BSC-1A5B9P-01
    bsx
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000
      ram name=download.ram size=0x40000
      map id=io address=00-3f,80-bf:5000-5fff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=40-5f,c0-ff:0000-ffff
      map id=rom address=78-7d:0000-ffff
      map id=ram address=20-3f:6000-7fff mask=0xe000
      map id=ram address=70-77:0000-ffff
  information
    title:    BS-X それは名前を盗まれた待の物語
    name:     BS-X - Sore wa Namae o Nusumareta Machi no Monogatari
    region:   JP
    revision: 1.1
    board:    BSC-1A5B9P-01
    serial:   SHVC-ZBSJ-JPN
    sha256:   3ce321496edc5d77038de2034eb3fb354d7724afd0bc7fd0319f3eb5d57b984d
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000
      ram name=download.ram size=0x40000

release
  cartridge region=NTSC
    board type=LDH3C revision=01
    spc7110
      rom name=program.rom size=0x100000
      rom name=data.rom size=0x400000
      ram name=save.ram size=0x2000
      map id=io address=00-3f,80-bf:4800-483f
      map id=io address=50,58:0000-ffff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff mask=0xe000
    epsonrtc
      ram name=rtc.ram size=0x10
      map id=io address=00-3f,80-bf:4840-4842
  information
    title:    Far East of Eden 天外魔境 Zero
    name:     Far East of Eden - Tengai Makyou Zero
    region:   JP
    revision: 1.0
    board:    SHVC-LDH3C-01
    serial:   SHVC-AZRJ-JPN
    sha256:   8620203da71d32d017bb21f542864c1d90705b87eb67815d06b43f09120318aa
    configuration
      rom name=program.rom size=0x100000
      rom name=data.rom size=0x400000
      ram name=save.ram size=0x2000
      ram name=rtc.ram size=0x10

release
  cartridge region=NTSC
    board type=LDH3C revision=01
    spc7110
      rom name=program.rom size=0x100000
      rom name=data.rom size=0x400000
      ram name=save.ram size=0x2000
      map id=io address=00-3f,80-bf:4800-483f
      map id=io address=50,58:0000-ffff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff mask=0xe000
    epsonrtc
      ram name=rtc.ram size=0x10
      map id=io address=00-3f,80-bf:4840-4842
  information
    title:    Far East of Eden 天外魔境 Zero 少年ジャンプの章
    name:     Far East of Eden - Tengai Makyou Zero - Shounen Jump no Shou
    region:   JP
    revision: 1.0
    board:    SHVC-LDH3C-01
    serial:   SHVC-AZQJ-JPN
    sha256:   69d06a3f3a4f3ba769541fe94e92b42142e423e9f0924eab97865b2d826ec82d
    configuration
      rom name=program.rom size=0x100000
      rom name=data.rom size=0x400000
      ram name=save.ram size=0x2000
      ram name=rtc.ram size=0x10

release
  cartridge region=NTSC
    board type=1DS0B revision=10,20
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    necdsp model=uPD96050 frequency=11000000
      rom name=st011.program.rom size=0xc000
      rom name=st011.data.rom size=0x1000
      ram name=save.ram size=0x1000
      map id=io address=60-67,e0-e7:0000-3fff select=0x0001
      map id=ram address=68-6f,e8-ef:0000-7fff
  information
    title:    早指し二段森田将棋
    name:     Hayazashi Nidan Morita Shougi
    region:   JP
    revision: 1.0
    board:    SHVC-1DS0B-10
    serial:   SHVC-2M
    sha256:   5b1a90c70df7537743900ad159e92ba3f72e4560934a64a46d62faba2ca83a70
    configuration
      rom name=program.rom size=0x80000
      rom name=st011.program.rom size=0xc000
      rom name=st011.data.rom size=0x1000
      ram name=save.ram size=0x1000

release
  cartridge region=NTSC
    board type=1DE3B revision=01
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=rom address=40-5f,c0-df:0000-ffff mask=0x8000
    map id=ram address=60-7d,e0-ff:0000-ffff
    armdsp frequency=21477272
      rom name=st018.program.rom size=0x20000
      rom name=st018.data.rom size=0x8000
      ram size=0x4000
      map id=io address=00-3f,80-bf:3800-38ff
  information
    title:    早指し二段森田将棋2
    name:     Hayazashi Nidan Morita Shougi 2
    region:   JP
    revision: 1.0
    board:    SHVC-1DE3B-01
    serial:   SHVC-A2MJ-JPN
    sha256:   3dd659d048aba8fc7d07b6ab726c9f711c948141e99e393685ae6cc311a83dc5
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000
      rom name=st018.program.rom size=0x20000
      rom name=st018.data.rom size=0x8000
      ram name=st018.program.ram size=0x4000

release
  cartridge region=NTSC
    board type=2P3B revision=01
    rom name=program.rom size=0x80000
    map id=rom address=00-0f,80-8f:8000-ffff
    map id=rom address=40-4f,c0-cf:0000-ffff
  information
    title:    くにおくんのドッジボールだよ 全員集合! トーナメント スペシャル
    name:     Kunio-kun no Dodge Ball da yo - Zen'in Shuugou! Tournament Special
    region:   JP
    revision: 1.0
    board:    SHVC-2P3B-01
    serial:   N/A
    sha256:   bcba4ca39f0279f7a52657bccbffa84564eaea455e2565597b93942ec245fdb1
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board id:BSC-1J3M-01
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff
    map id=rom address=40-5f,c0-df:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
    bsxslot
      map id=rom address=20-3f,a0-bf:8000-ffff
      map id=rom address=60-7d,e0-ff:0000-ffff
  information
    title:    鮫亀
    name:     Same Game
    region:   JP
    revision: 1.0
    board:    BSC-1J3M-01
    serial:   SHVC-ZS5J-JPN
    sha256:   3a709383208d8258dceb20a5e566903326515ba42931bf97fd389a415a13a72d
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board id:Sufami Turbo
    rom name=program.rom size=0x40000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    sufamiturbo
      slot id=A
        map id=rom address=20-3f,a0-bf:8000-ffff mask=0x8000
        map id=ram address=60-6f,e0-ef:0000-ffff
      slot id=B
        map id=rom address=40-5f,c0-df:0000-7fff mask=0x8000
        map id=rom address=40-5f,c0-df:8000-ffff mask=0x8000
        map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    スーファミターボ
    name:     Sufami Turbo
    region:   JP
    revision: 1.0
    board:    Sufami Turbo
    serial:   SFT-0100-JPN
    sha256:   edacb453da14f825f05d1134d6035f4bf034e55f7cfb97c70c4ee107eabc7342
    configuration
      rom name=program.rom size=0x40000

release
  cartridge region=NTSC
    board type=LJ3M revision=01
    rom name=program.rom size=0x600000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f:8000-ffff offset=0x400000
    map id=rom address=40-7d:0000-ffff offset=0x400000
    map id=rom address=80-bf:8000-ffff offset=0x000000 mask=0xc00000
    map id=rom address=c0-ff:0000-ffff offset=0x000000 mask=0xc00000
    map id=ram address=80-bf:6000-7fff mask=0xe000
  information
    title:    テイルズ オブ ファンタジア
    name:     Tales of Phantasia
    region:   JP
    revision: 1.0
    board:    SHVC-LJ3M-01
    serial:   SHVC-ATVJ-JPN
    sha256:   77b2d5450ce3c87185f913c2584673530c13dfbe8cc433b1e9fe5e9a653bf7d5
    configuration
      rom name=program.rom size=0x600000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    ヨッシーのクッキー クルッポンオーブンでクッキー
    name:     Yoshi no Cookie - Kuruppon Oven de Cookie
    region:   JP
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SHVC-YO
    sha256:   2b2fe61ac7a79c3cfaa0bc16f1b1f4da544fcc37cfdf3c18879d31b8b9f87941
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    3 Ninjas Kick Back
    name:     3 Ninjas Kick Back
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-A3NE-USA
    sha256:   2ffe8828480f943056fb1ab5c3c84d48a0bf8cbe3ed7c9960b349b59adb07f3b
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    The 7th Saga
    name:     7th Saga, The
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-01
    serial:   SNS-EL-USA
    sha256:   4dd631433c867ba920997fd3add2c838b62e70e06e0ef55c53884b8b68b0dd27
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Aaahh!!! Real Monsters
    name:     Aaahh!!! Real Monsters
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-ANNE-USA
    sha256:   ce164872c4f5814bce04cf0565edcdb5b7969ae95a3b5cd515cfb626b5cde7b3
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    ABC Monday Night Football
    name:     ABC Monday Night Football
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-10
    serial:   SNS-N5-USA
    sha256:   bb83f982961c33b81fefc1f545e18ab572d1c43cf6c241948544f05a1a71f2ba
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    ACME Animation Factory
    name:     ACME Animation Factory
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-11
    serial:   SNS-ACME-USA
    sha256:   d07e8802a6d9c777247874e05ec08fce7e0fa1bf122cc1ab9913f7d828e4072b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    ActRaiser
    name:     ActRaiser
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-11
    serial:   SNS-AR-USA
    sha256:   b8055844825653210d252d29a2229f9a3e7e512004e83940620173c57d8723f0
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    ActRaiser 2
    name:     ActRaiser 2
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-01
    serial:   SNS-A8-USA
    sha256:   71bdd448a30b88725864e55594ebb67a118b1f197a3f9e5dd39dbf23399efa15
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Addams Family
    name:     Addams Family
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-AF-USA
    sha256:   e645310d2406ace85523ed91070ee7ff6aa245217267dacb158ae9fc75109692
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Addams Family 2: Pugsley's Scavenger Hunt
    name:     Addams Family 2 - Pugsley's Scavenger Hunt
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-AH-USA
    sha256:   b6957bae7fd97ba681afbf8962fe2138e209649fd88ed9add2d5233178680aaa
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Addams Family Values
    name:     Addams Family Values
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-VY-USA
    sha256:   f59a0a8ed11ea2ba6217b1640e74bab8d8d8161a4585f5ae4a02edd7958ad9a3
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Adventures of Batman & Robin
    name:     Adventures of Batman & Robin, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ABTE-USA
    sha256:   8083307f3f4b7df9e5bf53d5f25877c2e548f0f677540d4ee62d60ccca3098f8
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Adventures of Dr. Franken
    name:     Adventures of Dr. Franken, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6F-USA
    sha256:   ecd964ae44e61203bc8759cfc6441365bf0c6e7bae6ad2a0fd553d4c7efab71e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Adventures of Kid Kleets
    name:     Adventure of Kid Kleets, The
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-YK-USA
    sha256:   670d898bdcf97d7ca3aab6c2dd1641f1270fcc2a070bbd3028ab413aef2b2ecc
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Adventures of Mighty Max
    name:     Adventures of Mighty Max, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AMOE-USA
    sha256:   889beb58d2a48a05a6230cabda14555cb030e2e986c0293bdf396e85af5c6798
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Adventures of Rocky and Bullwinkle and Friends
    name:     Adventures of Rocky and Bullwinkle and Friends, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-RZ-USA
    sha256:   b70099186d3774355ac5db370240e370c73f9ce5341f6c805cf9f771374b43ae
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Adventures of Yogi Bear
    name:     Adventures of Yogi Bear
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-Y8-USA
    sha256:   8049175767eddbc3e21ca5b94ee23fc225f834ccfab4ded30d2e981b0ef73ab6
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Aero Fighters
    name:     Aero Fighters
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-AERE-USA
    sha256:   b737de81315eef8ded7cfd5df6b37aba01d9e6e14566486db7ec83eb0c1aa85e
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Aero the Acro-Bat
    name:     Aero the Acro-Bat
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-XB-USA
    sha256:   18a553dafd4980cc2869180b05f9fdf6e980bf092cc683e498ec6373c9701c6e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Aero the Acro-Bat 2
    name:     Aero the Acro-Bat 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AE2E-USA
    sha256:   fc5df5db0a96d39d3df651f63993adf0f5cb5a6b92a36211f3a05d460d92c72d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Aerobiz
    name:     Aerobiz
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-AL-USA
    sha256:   d5f0fbeed3774bbccbd769698fc4051487a0a5eb699790a8a094451595600f60
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Aerobiz Supersonic
    name:     Aerobiz Supersonic
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-AG-USA
    sha256:   6b6921d7aba9ba353e96e39c8d79d24eef1b84eb5c7fa54edaa83d2447dcd954
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Air Cavalry
    name:     Air Cavalry
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ACCE-USA
    sha256:   1f5738552c51de25ffe8aa44ff396c1ab238435296f1e8f99f8cf335483c03d5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Air Strike Patrol
    name:     Air Strike Patrol
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-4A-USA
    sha256:   69c5805ad0494703e7d636d3d40d615d33e79bebef9d2cdb4a23b73d44c7b6f9
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Al Unser Jr.'s Road to the Top
    name:     Al Unser Jr.'s Road to the Top
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AUJE-USA
    sha256:   0099bdb56e4190f81440c8c29395ecc15d78eeabfc38a93dca4773817d6f720f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Aladdin
    name:     Aladdin
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-RJ-USA
    sha256:   aa768b8b00123717c8d49f2c6731cdbfd80ab6a54128bae7594e93f45e38a19e
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Alien 3
    name:     Alien 3
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-A3-USA
    sha256:   e637b8241d55ee334a3452888df5e30d72c520dbb55c498db1a984438bab3e55
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Alien vs. Predator
    name:     Alien vs. Predator
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-AP-USA
    sha256:   05eb897d7696555790591c431c9d55a43ff9a1c12162443c17c5fcddfa5eb3c5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    American Gladiators
    name:     American Gladiators
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AA-USA
    sha256:   dc9cefb4dd50dce2e9d626ac71d4a06306516cba4bc784c90e4a30fe4e7ff4ef
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    An American Tail: Fievel Goes West
    name:     American Tail - Fievel Goes West, An
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-9W-USA
    sha256:   6931a3eacb7ab3c2f2fb57ba7d59c6da56fe6c7d60484cebec9332e6caca3ae9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Andre Agassi Tennis
    name:     Andre Agassi Tennis
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-7A-USA
    sha256:   9abb426bac62e03e4437f5a9a8455c3000042f339125cc60ae29382ae89d8493
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Animaniacs
    name:     Animaniacs
    region:   NA
    revision: 1.0
    board:    MJSC-1A0N-30-2
    serial:   SNS-ANCE-USA
    sha256:   626f1fe52b0df0f8ede23f0062cd842321c8dabf14aefdca12e526876ecf383a
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Arcade's Greatest Hits: The Atari Collection 1
    name:     Arcade's Greatest Hits - The Atari Collection 1
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-AW7E-USA
    sha256:   31569bef662bc438194726967970bf19f504101060763cbd649350362fb6ef2f
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Arcana
    name:     Arcana
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-RF-USA
    sha256:   aac9d6f2b8408e4bbdc83ebbba755428caf8021fefbfac7220fb4c772abd9944
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ardy Light Foot
    name:     Ardy Light Foot
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A9-USA
    sha256:   0f474dafe5a26f3dea491d18073dd490d2f1f91313a7e91086565510d38d9a09
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Arkanoid: Doh It Again!
    name:     Arkanoid - Doh It Again!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-A6-USA
    sha256:   14d3ece30898587eda20c661a4a55ec595ba6352ca1f0bfc177542aa0eef0039
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Art of Fighting
    name:     Art of Fighting
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-01
    serial:   SNS-RW-USA
    sha256:   55e57c5e012583ff0fafd1aee16b3f8269ee2b34fe10f10b93ba0dde72f2b78d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Axelay
    name:     Axelay
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-AX-USA
    sha256:   2431f8dc067ba27c6c3a846929f3deac6a45aa53a9a9ac20ede8ec5ca6854ea2
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1K0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1b.program.rom size=0x1800
      rom name=dsp1b.data.rom size=0x800
      ram size=0x200
      map id=io address=00-1f,80-9f:6000-7fff select=0x1000
  information
    title:    Ballz 3D
    name:     Ballz 3D
    region:   NA
    revision: 1.0
    board:    SHVC-1K0N-01
    serial:   SNS-ABZE-USA
    sha256:   a3213e40d7abbc25a5b909642433103b06d8f90500c930bf64093ade0329da78
    configuration
      rom name=program.rom size=0x100000
      rom name=dsp1b.program.rom size=0x1800
      rom name=dsp1b.data.rom size=0x800
      ram name=dsp1b.data.ram size=0x200

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Barbie Super Star
    name:     Barbie Super Star
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-8L-USA
    sha256:   865919b25a9d241c907bcda18b380e3c704f33f4997ad44559046f0f08c4968b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Barkley Shut Up and Jam!
    name:     Barkley Shut Up and Jam!
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-U5-USA
    sha256:   fe1ad128313b2b9a47f89cf0d95d4c0cc2cb35a817ac5d915ee6c4d98d47d675
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bass Masters Classic
    name:     Bass Masters Classic
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-20
    serial:   M/SNS-ABAE-USA
    sha256:   c670e16c076f6d92ba581b78769b604e79ad567c04c647dac557f45a48c3448f
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bass Masters Classic: Pro Edition
    name:     Bass Masters Classic - Pro Edition
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A9BE-USA
    sha256:   db1ac03cc8b7daaa812da239029bcf999b30b2afe1c03d51f7ae849a796617ea
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Bassin's Black Bass
    name:     Bassin's Black Bass
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-AB2E-USA
    sha256:   e2be173c77bd1957787be36d13334f655e14d32dad99cacb0fd5e5fc65d96fa1
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Batman Forever
    name:     Batman Forever
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-A3BE-USA
    sha256:   e36aaba64be016cabc33a2dcf88913341e2edacc722e2a1ebe04eccda2a5d6e7
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Batman Returns
    name:     Batman Returns
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-BJ-USA
    sha256:   f8d5c51f74df33edc827fbf8df7aab70160770ab0a896db6e59438ad9208cc6e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Battle Blaze
    name:     Battle Blaze
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-BZ-USA
    sha256:   983022203546e031773db0d1af7552c489187954882843f13ff123f09064c1d3
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Battle Cars
    name:     Battle Cars
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-4B-USA
    sha256:   d19113964c4d268e986018e256e9358cde9a23a05e6053b54c0f2d950dcdf395
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Battle Clash
    name:     Battle Clash
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-BT-USA
    sha256:   32f42fda0667d9435a2de84c7ce7b067bcbab1164f8f6d837992ad6cfac4f8de
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Battle Grand Prix
    name:     Battle Grand Prix
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-BG-USA
    sha256:   6e436020d967d35a0f7ce43e38e83a616b70696ae7d35b37cd56601cfb3822ba
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Battletoads & Double Dragon
    name:     Battletoads & Double Dragon
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-1
    serial:   SNS-UL-USA
    sha256:   c7f0269498d190e4fd0f6f880a148fbe3713cd3a632083bac1e5bd18f8172371
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Battletoads in Battlemaniacs
    name:     Battletoads in Battlemaniacs
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-NX-USA
    sha256:   b0dbd4d7e5689c32234e80b0c5362ef67c425ab72d6ddb49d1cb1133ef630ef7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bazooka Blitzkrieg
    name:     Bazooka Blitzkrieg
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-BY-USA
    sha256:   0b0f991da7ce4b3c2974d6adf62191ec554db9793c5def14cdfb62b7ae28cc98
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Beauty and the Beast
    name:     Beauty and the Beast
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-EW-USA
    sha256:   d7271ca08400bbf5ae165b0aaa6e8a8a1b266f72e6e0ae10aae529732a472f7c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Beavis and Butt-Head
    name:     Beavis and Butt-Head
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-ABUE-USA
    sha256:   15d4fc90cb202a9391718cd40b9f0384165aef03018ed932540e8f7c18b397dd
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bebe's Kids
    name:     Bebe's Kids
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6B-USA
    sha256:   4958eda26f2419f473449017c64121caee5e49c480ffa205422e7dd45cd23e31
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Beethoven: The Ultimate Canine Caper!
    name:     Beethoven - The Ultimate Canine Caper!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-2V-USA
    sha256:   4d22279e534848012e0f1595468687742ae18cabc3fe44eeef938bc3a4dd08bf
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Best of the Best: Championship Karate
    name:     Best of the Best - Championship Karate
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-BE-USA
    sha256:   e4e9beaeeb3e968af772d1c4c9e4c1b4dfdba4e47c0205b458e1ab3a62a96060
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Big Sky Trooper
    name:     Big Sky Trooper
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-20
    serial:   M/SNS-AB9E-USA
    sha256:   4cb601168c91fa0608c16a8cf2f292d991c6a5615d51861dee2f9b91c8d6bb19
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Biker Mice from Mars
    name:     Biker Mice from Mars
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ABME-USA
    sha256:   91ba5691dea3cdf103177ae5779110fc372fce8229cf91f263073667e7a8b5b7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Bill Laimbeer's Combat Basketball
    name:     Bill Laimbeer's Combat Basketball
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-C8-USA
    sha256:   6fa6b8a8804ff6544bdedf94339a86ba64ce0b6dbf059605abb1cd6f102d3483
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Bill Walsh College Football
    name:     Bill Walsh College Football
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-7F-USA
    sha256:   ec2d91e238c26a5ddf7067d104b3b3e2eaee89255377e1eb6c4df8f301300e64
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bio Metal
    name:     Bio Metal
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-BV-USA
    sha256:   de1de85ad549a6aaf0431cceb47cbd07e1f6e81f9e16fd62575305e2c1f06240
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Blackthorne
    name:     Blackthorne
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6Z-USA
    sha256:   328c8f57e2ea371f6fd5b8a9834c56e35eb3bfe710502dd80f370739f9ccb7e1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    BlaZeon: The Bio-Cyborg Challenge
    name:     BlaZeon - The Bio-Cyborg Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-BL-USA
    sha256:   0d4e0d134396bd1c7254cdc1da356eb944ca14910b6690f484a75a9c3a8106e7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Blues Brothers
    name:     Blues Brothers, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-B6-USA
    sha256:   99f40f06fa4dbeeea4fe67d2de5b4c1bf301bedac1958ba1c239dcaf39b0a998
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    B.O.B.
    name:     BOB
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-B4-USA
    sha256:   9e6ebebcf14609c2a38a5f4409d0c8c859949cded70c5b6fd16fd15d9983d9d3
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bonkers
    name:     Bonkers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ABNE-USA
    sha256:   854d2492d1cb059749bb0904ca5f92a5eeec09167abf84f7cca4023b1819e4f0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Boogerman: A Pick and Flick Adventure
    name:     Boogerman - A Pick and Flick Adventure
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AB4E-USA
    sha256:   8f131182b286bd87f12cf4f00453336538ce690d0e1f0972ac0be98df4d48987
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Boxing Legends of the Ring
    name:     Boxing Legends of the Ring
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LL-USA
    sha256:   e67940a2106c1507f3a8d38790f263bbbf814578ebf3dbc4e3eb6007d310793c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Brain Lord
    name:     Brain Lord
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-11
    serial:   SNS-3B-USA
    sha256:   f4666355e7fea434843dc6d5119673bd6c23e69b884aac0b382ff036997e52b5
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Brainies
    name:     Brainies, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-B7-USA
    sha256:   9885ca148d32c4df6230642bcfa153f7e51b9559415042a831db14d07b3e6c3d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bram Stoker's Dracula
    name:     Bram Stoker's Dracula
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-5D-USA
    sha256:   bbde8b46c7262f9d4a5b3926a00850cb00b4f7711f6421f0adf4e2b0c847a5d6
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J5M revision=01
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,90-9f:6000-7fff mask=0xe000
    map id=ram address=30-3f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Brandish
    name:     Brandish
    region:   NA
    revision: 1.0
    board:    SHVC-2J5M-01
    serial:   SNS-QF-USA
    sha256:   130a74e76369b0ec4d6378a014550921433f1ae1ac1dddffb51f77c9f21a818f
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Brawl Brothers
    name:     Brawl Brothers
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-10
    serial:   SNS-RE-USA
    sha256:   044b61613ed66eae08abd5fa5dcd13b24aab11a942e3309cdff624d198c47440
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    BreakThru!
    name:     BreakThru!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ABXE-USA
    sha256:   aad8c9be1b7a9662256b0c3d76f5b7a273bcd497aa838232d307e9f2e80cf699
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Breath of Fire
    name:     Breath of Fire
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-11
    serial:   SNS-BF-USA
    sha256:   cbc496a7879ba78f32c51c3df4ba1a1a42f17d78d48a39ea9c709d1ad18cb8df
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Breath of Fire II
    name:     Breath of Fire II
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-AF2E-USA
    sha256:   fede9d4aec8c35ed11e2868c3c517bce53ee3e6af724085c92500e99e43e63de
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Brett Hull Hockey
    name:     Brett Hull Hockey '94
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-01
    serial:   SNS-5Y-USA
    sha256:   99450b45ccc70a1ed387f968c8f863a3c7f6a4b86809e841c25a71e1e904ac61
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Brett Hull Hockey '95
    name:     Brett Hull Hockey '95
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ABHE-USA
    sha256:   a427db4860cb5140935f263ba872fe14949c3548db644fed46b2edf3dff3d4a8
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bronkie the Bronchiasaurus
    name:     Bronkie the Bronchiasaurus
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AB6E-USA
    sha256:   f2bc4cb460dfc5d5288065a2f529190b72e69d4e02634246086244c20f30521c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Brunswick World Tournament of Champions
    name:     Brunswick World Tournament of Champions
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-AWUE-USA
    sha256:   ee00c57ddfa9b967d1857acb518df039ba73055bdebe78db014de0f5da262fd9
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Brutal: Paws of Fury
    name:     Brutal - Paws of Fury
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ABLE-USA
    sha256:   9e3ad5e521e759a2e2260ea8072eb3314b6fcf67a3b7247357a5de9bcb24eeaa
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bubsy in Claws Encounters of the Furred Kind
    name:     Bubsy I - Claws Encounters of the Furred Kind
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-UY-USA
    sha256:   811cbc3287c0959e8eb242e817684d36de664ebebc5873a1fa9958693857c438
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Bubsy II
    name:     Bubsy II
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-ABBE-USA
    sha256:   2357d344af77d25dda030520ce203045fd9060f83e3b9609a228dba859d9017b
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bugs Bunny: Rabbit Rampage
    name:     Bugs Bunny - Rabbit Rampage
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-R7-USA
    sha256:   49020695a017acc3dfadea97a60e28609e583571f69c5abeb3c6b1c2db8113fa
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bulls vs. Blazers and the NBA Playoffs
    name:     Bulls vs. Blazers and the NBA Playoffs
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-BU-USA
    sha256:   ba4f31353e0e1233b574391ad97a80901d7de212e2c55d7be2af11a9a57c8225
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bulls vs. Blazers and the NBA Playoffs
    name:     Bulls vs. Blazers and the NBA Playoffs
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-20
    serial:   SNS-BU-USA-1
    sha256:   d8b3d9267470adb8ef2d197ade44476e504c0823f8fe9d2cf2883a03aa75bd49
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Bust-a-Move
    name:     Bust-a-Move
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AYKE-USA
    sha256:   d6f6c30732dae8d00cd83628c3156acbdf26f99df701f779522e21de74dae5fe
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cacoma Knight in Bizyland
    name:     Cacoma Knight in Bizyland
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-CC-USA
    sha256:   a8294d449bbb8370816efe0374704e8af20dbbde9c19afe969d898528bc293d0
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cal Ripken Jr. Baseball
    name:     Cal Ripken Jr. Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-CJ-USA-1
    sha256:   75993076c8773e7c7d555ef290be10590def40ca7b83307b8dc86556b04a6565
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    California Games II
    name:     California Games II
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-C2-USA
    sha256:   19f4a1f08aa55ff3cc8ff7ae60ffe03f0e436e8d8901455f1311f2276497a492
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A1M revision=01
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Cannondale Cup
    name:     Cannondale Cup
    region:   NA
    revision: 1.0
    board:    SHVC-2A1M-01
    serial:   SNS-ASCE-USA
    sha256:   6cdec3cb9c91aa23feb13005f874bda580c2f548302874491a31951031c9dbbd
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Capcom's MVP Football
    name:     Capcom's MVP Football
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-NL-USA
    sha256:   9590110a990e90f525d5c8d70fc2a3da10879378003173b6761afb8bf042ee0d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Capcom's Soccer Shootout
    name:     Capcom's Soccer Shootout
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-JL-USA
    sha256:   fd5761f9dd1f2b87ad11df6085046d0dfcdc3a79139263e47b0cff707966ba51
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Captain America and The Avengers
    name:     Captain America and The Avengers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6A-USA
    sha256:   2a117951adcfbc4298763673a834d502c3f7a3964db1e59650f113c07bb831fb
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Captain Commando
    name:     Captain Commando
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-QM-USA
    sha256:   d9b7f9356be0780f0037093a86ef8450f15e569cbd3680073d1cd345dfadb709
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Captain Novolin
    name:     Captain Novolin
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CP-USA
    sha256:   8784614896e2b3e8d98c8166613ca5d2329643795a4dc107791c58c6c51e1268
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Carrier Aces
    name:     Carrier Aces
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ACAE-USA
    sha256:   ee5fc27dd19a2ecb3c3c7c73d558a18ffd5ff365710c18b88150e277f08d587e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Casper
    name:     Casper
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AXCE-USA
    sha256:   b9b982cd8f91c51089d49b550f11882b1ee785ebddcb7355cfc465916d61a042
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Castlevania: Dracula X
    name:     Castlevania - Dracula X
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ADZE-USA
    sha256:   367725a149a471411e4f72ad77603b61fb101c9cab4521be5647e13708cc97ba
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Champions World Class Soccer
    name:     Champions World Class Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8W-USA
    sha256:   32008c429eafe7eb59aff7a89f77e1a267f02f9061ff8830ade7b99081e27f7c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Championship Pool
    name:     Championship Pool
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-5P-USA
    sha256:   e6c4f468b39f2dea3436b63758db8ac9b29731357b982ec373334a36f202623f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Championship Soccer '94
    name:     Championship Soccer '94
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-67-USA
    sha256:   fdd763dffc6fb38975d4c6d6e3123f9738122781b7d13e1fc7f9820464cbaeb5
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Chavez
    name:     Chavez
    region:   MX
    revision: 1.0
    board:    SHVC-1A1M-01
    serial:   SNS-ZV-USA
    sha256:   aa69d4e19c2eb206fe88eba65994c830256c220e5506f59824aefa0a75dd44d5
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Chavez II
    name:     Chavez II
    region:   MX
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AC2E-USA
    sha256:   39de64101cf7f25863ce55e03a27d13422ea572ee996746578b5936fea80228b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Chessmaster
    name:     Chessmaster, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-CH-USA
    sha256:   ee0e51d922d1cf8abe3dfc6b0d84a988a6635dc96b2a96962007c41aaa542774
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Chester Cheetah: Too Cool to Fool
    name:     Chester Cheetah - Too Cool to Fool
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CE-USA
    sha256:   c7e7df8932bf0056aa530f3dc3c913c1171a359af4c197094c2b972946dc6051
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Chester Cheetah: Wild, Wild Quest
    name:     Chester Cheetah - Wild, Wild Quest
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-7C-USA
    sha256:   21a2aa488cb8140ca318f7d1f513103d14e758181aa336a594097d32ba0a7587
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Choplifter III
    name:     Choplifter III
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-3C-USA-1
    sha256:   9a064b67f522b75b82d0857519c0e33b4dbbe494c2ef79a44fdc913d605d0b26
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Chrono Trigger
    name:     Chrono Trigger
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-ACTE-USA
    sha256:   06d1c2b06b716052c5596aaa0c2e5632a027fee1a9a28439e509f813c30829a9
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Chuck Rock
    name:     Chuck Rock
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CK-USA
    sha256:   63ab79e86ea13e2cf9bb67aec971febb68450db9865b00b5f412610653822393
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Clay Fighter
    name:     Clay Fighter
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-01
    serial:   SNS-8C-USA
    sha256:   8b7525b2aa30cbea9e3deee601dd26e0100b8169c1948f19866be15cae0ac00d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Clay Fighter: Tournament Edition
    name:     Clay Fighter - Tournament Edition
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-7E-USA
    sha256:   ea52b39a8e1ea15bfad6b92883c9a5fe744cecd7c0e175aa3bd40280cf7a966e
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Clay Fighter 2: Judgment Clay
    name:     Clay Fighter 2 - Judgment Clay
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-20
    serial:   SNS-ACZE-USA
    sha256:   2d40c86bc19d85555bf2672acf515b04dbf56a6a59b29ad503e672310b0fae3b
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Claymates
    name:     Claymates
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-Y5-USA
    sha256:   6d9cd7f0cda3c0a82ed3f6a92cbbba4fe8365438e0a7867ad1cae2044d1738eb
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cliffhanger
    name:     Cliffhanger
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6C-USA
    sha256:   e5980b990605a9c91fa89101c440b2ec9993329296ba09a9538042d724a080fb
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Clue
    name:     Clue
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CL-USA
    sha256:   03f6c69aef92d36b5ea25a6023368da0e1da9fa160e8316ebd533d4f358ffacf
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    College Football USA '97: The Road to New Orleans
    name:     College Football USA '97 - The Road to New Orleans
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   M/SNS-AC7E-USA
    sha256:   5536cea2da39f2572abe3b0fcf71f8fcd981376b470b174969772aae4a7a1845
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    College Slam
    name:     College Slam
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   M/SNS-ANYE-USA
    sha256:   b0be35a0d5e500f4fffca5f2940e0ec52c81ce99dacd773c3ca9cf92f592d943
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Combatribes
    name:     Combatribes, The
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-01
    serial:   SNS-CR-USA
    sha256:   c88a882ad72dfa07a9b1eb8a2183aa10057d60877a02edf90cf2cd8c78abe65e
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Congo's Caper
    name:     Congo's Caper
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-J2-USA
    sha256:   26e09f5bc2bde28d57aeca0bf5be7f7fb8e3b3887af975bcbf2e6f29b07df56f
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Contra III: The Alien Wars
    name:     Contra III - The Alien Wars
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CS-USA
    sha256:   a93ea87fc835c530b5135c5294433d15eef6dbf656144b387e89ac19cf864996
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cool Spot
    name:     Cool Spot
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-C8-USA
    sha256:   c7d622391f7699fb0dc6367e141c894e700cc9bd8abca69f36785e7bc2f42a49
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cool World
    name:     Cool World
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CD-USA
    sha256:   9674cc269d89a52d1719a487b44acf004fb777cbd58d76b19a2cd25749728215
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    CutThroat Island
    name:     CutThroat Island
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AC8E-USA
    sha256:   7c722f9941957630467c1784d0eb3f92fbfc0a2a1da3c8f5c27f593eca2a5a04
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cyber Spin
    name:     Cyber Spin
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CF-USA
    sha256:   c4ae2797fac2586b8640064be6398f2b4f2b3158a07f26c66912b29f7fd197de
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Cybernator
    name:     Cybernator
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-AV-USA
    sha256:   ad31b94ce928ecff605e2b89082154671691509e95d38370ed381437f2c36698
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    D-Force
    name:     D-Force
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-DF-USA
    sha256:   337e643d3e63915de06429992f306e8d2b73aed6849b795f9c855c2d03c18180
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Daffy Duck: The Marvin Missions
    name:     Daffy Duck - The Marvin Missions
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-YF-USA
    sha256:   4068add412571bd85adac32dff9835e4a4886077d752adb104fee3908e42b7ef
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Darius Twin
    name:     Darius Twin
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-DT-USA
    sha256:   ceb470157576eac3b8b8c16e8ab6b59672409681ffb4232e4ec39dd0cb37ef91
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    David Crane's Amazing Tennis
    name:     David Crane's Amazing Tennis
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ZT-USA
    sha256:   e6efb6361af04963f22c772f879a466543f56b3b6a084204fef2dcb4659d82d9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Death and Return of Superman
    name:     Death and Return of Superman, The
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-30
    serial:   M/SNS-9D-LTN
    sha256:   e678a29a93111cf2016c487ba9977b14de8f719040651a42c74bd74eea2d0d81
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Demolition Man
    name:     Demolition Man
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AD6E-USA
    sha256:   752d24fab240f4dd1dfbfea5ec83438998316806ad44488bf8c84430ca5a2cd0
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Demon's Crest
    name:     Demon's Crest
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-3Z-USA
    sha256:   18d40a807d5f88c5b6a1ad849eec7e0f189225d9a1586037c850f6680b5844de
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Dennis the Menace
    name:     Dennis the Menace
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-4D-USA
    sha256:   a362033d0d7e754d79202b255679186ad799b9e784719614b913ec8c9857ae33
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Desert Strike: Return to the Gulf
    name:     Desert Strike - Return to the Gulf
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-RG-USA
    sha256:   606abf536440173ae36466db360c7db6b474beb7a105c8a62bc74a54cbe1c38b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Dig & Spike Volleyball
    name:     Dig & Spike Volleyball
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-VH-USA
    sha256:   9c4721339a197185d61e0697ea0149db143a27ddb2f57ebd398f18bcf4d7724b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    DinoCity
    name:     DinoCity
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-DW-USA
    sha256:   7dbfc44d28a46e6d399628e43086aa9fd0b2abeda4c108751a5ad91c102c3aaf
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1CA0N6S revision=01
    superfx revision=4
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x10000
      map id=io address=00-3f,80-bf:3000-34ff
      map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
      map id=rom address=40-5f,c0-df:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff size=0x2000
      map id=ram address=70-71,f0-f1:0000-ffff
  information
    title:    Dirt Trax FX
    name:     Dirt Trax FX
    region:   NA
    revision: 1.0
    board:    SHVC-1CA0N6S-01
    serial:   SNS-AF9E-USA
    sha256:   c5474de3592e2a99aac0e5511516658f9f0b50167c754a018409842ec35bcd45
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x10000
      superfx revision=4

release
  cartridge region=NTSC
    board type=1J1M revision=11,20
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x800
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Donkey Kong Country
    name:     Donkey Kong Country
    region:   NA
    revision: 1.1
    board:    SHVC-1J1M-11
    serial:   SNS-8X-USA
    sha256:   df2644d435330192a13768cc1f79c5aa3084a64217a5250c6dd4ffdbe2175be4
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1J1M revision=11,20
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x800
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Donkey Kong Country
    name:     Donkey Kong Country
    region:   NA
    revision: 1.2
    board:    SHVC-1J1M-20
    serial:   SNS-8X-USA
    sha256:   628147468c3539283197f58f03b94df49758a332831857481ea9cc31645f0527
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1J1M revision=11,20
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x800
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Donkey Kong Country 2: Diddy's Kong Quest
    name:     Donkey Kong Country 2 - Diddy's Kong Quest
    region:   NA
    revision: 1.0
    board:    SHVC-1J1M-20
    serial:   SNS-ADNE-USA
    sha256:   35421a9af9dd011b40b91f792192af9f99c93201d8d394026bdfb42cbf2d8633
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1J1M revision=11,20
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x800
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Donkey Kong Country 3: Dixie Kong's Double Trouble
    name:     Donkey Kong Country 3 - Dixie Kong's Double Trouble
    region:   NA
    revision: 1.0
    board:    SHVC-1J1M-20
    serial:   M/SNS-A3CE-USA
    sha256:   2277a2d8dddb01fe5cb0ae9a0fa225d42b3a11adccaeafa18e3c339b3794a32b
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1CB0N7S revision=01
    superfx revision=4
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x10000
      map id=io address=00-3f,80-bf:3000-34ff
      map id=rom address=00-3f:8000-ffff mask=0x8000
      map id=rom address=40-5f:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff size=0x2000
      map id=ram address=70-71:0000-ffff
  information
    title:    Doom
    name:     Doom
    region:   NA
    revision: 1.0
    board:    SHVC-1CB0N7S-01
    serial:   SNS-AD8E-USA
    sha256:   d45e26eb10c323ecd480e5f2326b223e29264c3adde67f48f0d2791128e519e8
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x10000
      superfx revision=4

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Doomsday Warrior
    name:     Doomsday Warrior
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-DM-USA
    sha256:   bb915b286b33842e39e9022366169233a4041515c7ecc60ac428420b28e48dd5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Double Dragon V: The Shadow Falls
    name:     Double Dragon V - The Shadow Falls
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-5E-USA
    sha256:   b32aa9cbf8f6baacc84f6418fa6fbc33b9ce71458fecc91275aafdbf6f743a99
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Dragon: The Bruce Lee Story
    name:     Dragon - The Bruce Lee Story
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AD5E-USA
    sha256:   d98d7da1e7636e067563e2e480d7dfbc013b7e9bdf1329fd61c5cacac0293e1d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Dragon View
    name:     Dragon View
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-ADVE-USA
    sha256:   a3b1cae3fe55fe52c035ab122e7f850640b0935524496d45b1915ca3c8a934f4
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Dragon's Lair
    name:     Dragon's Lair
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-DI-USA
    sha256:   49a1f9f5e6084b3daa1b13ab5a37ebe8bd3cf20e1c7429fbf722298092893e81
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Drakkhen
    name:     Drakkhen
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-11
    serial:   SNS-DK-USA
    sha256:   74910ce01d19d25cb97a243a51f21c3d522f02fb116682f60440da3292bb14f7
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Dream T.V.
    name:     Dream TV
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-VE-USA
    sha256:   1a79d51a2ad7dd4848205a07ff8e5d873b155dc420de5e52158c9bab935e05c3
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1B5B revision=02
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x8000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp2.program.rom size=0x1800
      rom name=dsp2.data.rom size=0x800
      ram size=0x200
      map id=io address=20-3f,a0-bf:8000-ffff select=0x4000
  information
    title:    Dungeon Master
    name:     Dungeon Master
    region:   NA
    revision: 1.0
    board:    SHVC-1B5B-02
    serial:   SNS-V2-USA
    sha256:   03d25227fb88899d0f3234c4a3f76f1dbe8d582cb6429454fd6f1c4cf14d5c6e
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000
      rom name=dsp2.program.rom size=0x1800
      rom name=dsp2.data.rom size=0x800
      ram name=dsp2.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Earth Defense Force
    name:     Earth Defense Force
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-ED-USA
    sha256:   0408e3d9f2259044344a3bfbd7a7ca3c3427f82108fbecd6e5c4c41e80cd303b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    EarthBound
    name:     EarthBound
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   SNS-MB-USA
    sha256:   a8fe2226728002786d68c27ddddf0b90a894db52e4dfe268fdf72a68cae5f02e
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Earthworm Jim
    name:     Earthworm Jim
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-AEJE-USA
    sha256:   4579e437279f79eedd6b9cf648a814df2ab2c83d937a1bcec1578d28965fb9a0
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Earthworm Jim 2
    name:     Earthworm Jim 2
    region:   NA
    revision: 1.0
    board:    MJSC-1J0N-20-2
    serial:   SNS-A2EE-USA
    sha256:   10eadaab168707829418702386e1bcedd2619d9bbefc37cf31c4118313bcf6de
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Eek! The Cat
    name:     Eek! The Cat
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-E7-USA
    sha256:   5d658b63d35e2e0baf48ae3bb04ea5e1553855b34bb39fc2c7ca41fbd3894d52
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Elite Soccer
    name:     Elite Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-L7-USA
    sha256:   b296054effb1948039635044bc905fdf8ff53e7a73038fd5d8436a913ea5ad8a
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Emmitt Smith Football
    name:     Emmitt Smith Football
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   M/SNS-AESE-USA
    sha256:   f14e30ee452ec930a6d08094094b287d0c40c8108f2017c418015242987649b3
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Equinox
    name:     Equinox
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-EX-USA
    sha256:   cfd666f0bbabec59613d9fe189db7d0a060a78047bc084c0c365840769047bf2
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    ESPN Baseball Tonight
    name:     ESPN Baseball Tonight
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-EV-USA
    sha256:   c702c3860e9dffdaa1917d013ecbcefdd2c47f89726992f7f810d879772dcc4d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    ESPN National Hockey Night
    name:     ESPN National Hockey Night
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-AEHE-USA
    sha256:   3326061160ad12c7aef5176544ec1c8ef2f534a51960ca882dbc7fcb9b1a7384
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    ESPN Speed World
    name:     ESPN Speed World
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-ASWE-USA
    sha256:   7b2fdab972e393395f5379d7fb13e40464db152f6acf58b2d2a6a18f81cefecb
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    ESPN Sunday Night NFL
    name:     ESPN Sunday Night NFL
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-ASNE-USA
    sha256:   faf595b4671f005fa9367ba3fcd30dbff371bc7a9ca8bbfbc0ebfcc5826b60f8
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A3M revision=01 mapper=MAD-R
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    E.V.O.: Search for Eden
    name:     EVO - Search for Eden
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-01
    mapper:   MAD-R
    serial:   SNS-46-USA
    sha256:   8481e47381bd98c27b9782b5727a5d5f0976fbb3aa3df25c2c42aa37e0586815
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Exertainment Mountain Bike Rally
    name:     Exertainment Mountain Bike Rally
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-9X-USA
    sha256:   f2455253db316b6ccd0c5c686d1f1e2d94cd5e37534e70a3a07a409120d58df6
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Exertainment Mountain Bike Rally + Speed Racer
    name:     Exertainment Mountain Bike Rally + Speed Racer
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   SNS-ALFE-USA
    sha256:   a0521f50b0d0bff6666bfb712498476eb8d5974ef38caf157e2f67cbce5475bb
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Extra Innings
    name:     Extra Innings
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-GL-USA
    sha256:   1576066e0cb771a91caf79e7d4f6dc00eb0daa47f0786f1604b32537429a7f45
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Eye of the Beholder
    name:     Eye of the Beholder
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-01
    serial:   SNS-IB-USA
    sha256:   0a8cd5101f849ccd4e40d55fdc4edce914b2825b69eb76ec31cf53b59719e79e
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    F-Zero
    name:     F-Zero
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-04
    serial:   SNS-FZ-USA
    sha256:   bf16c3c867c58e2ab061c70de9295b6930d63f29f81cc986f5ecae03e0ad18d2
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    F1 Pole Position
    name:     F1 Pole Position
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-01
    serial:   SNS-6P-USA
    sha256:   55c33efb514568c9946c4b866c160ed190fe436faee265ee2fb107f7fe94d524
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    F1 Race of Champions
    name:     F1 Race of Champions
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-EH-USA
    sha256:   1d38e3af9e3a6409e93f4705b68c42558f558c68f3e83ef2a40e46cf560b26cc
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1DS0B revision=10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    necdsp model=uPD96050 frequency=11000000
      rom name=st010.program.rom size=0xc000
      rom name=st010.data.rom size=0x1000
      ram name=save.ram size=0x1000
      map id=io address=60-67,e0-e7:0000-3fff select=0x0001
      map id=ram address=68-6f,e8-ef:0000-7fff
  information
    title:    F1 Race of Champions II
    name:     F1 Race of Champions II
    region:   NA
    revision: 1.0
    board:    SHVC-1DS0B-20
    serial:   SNS-E2-USA
    sha256:   3ae7bfd38a3dc273f4d387af3b15621beefebf5056731af06e3822f5e57db5c5
    configuration
      rom name=program.rom size=0x100000
      rom name=st010.program.rom size=0xc000
      rom name=st010.data.rom size=0x1000
      ram name=save.ram size=0x1000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Faceball 2000
    name:     Faceball 2000
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-2F-USA
    sha256:   d689392884df91c2bb84b1411a96f3919b6c9cc8a583dff901a98f0d86d31c30
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Family Dog
    name:     Family Dog
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-D8-USA
    sha256:   2891f1eab285133364ecc379a5c9e1d0026d60f425f1a458d149014f386cfa50
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Family Feud
    name:     Family Feud
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-FN-USA
    sha256:   4f43ce12e1d8cd195468d7048494ad2930721e5bf9e69bfd86eeee707ffc634b
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Family Feud
    name:     Family Feud
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30
    serial:   SNS-FN-USA
    sha256:   d5c651d726dd957b80d03ab6fdbed4cdd26a3cccf5ec9d91af67251b3ec26a3c
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Fatal Fury
    name:     Fatal Fury
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-01
    serial:   SNS-GN-USA
    sha256:   c92f389d25870aada3002775838ec9f69a988120c0238af885fd08d46bd94930
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x280000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Fatal Fury 2
    name:     Fatal Fury 2
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-DJ-USA
    sha256:   a0c554d46034caef231c36dd6849828ca39703678fb7fdd15a11f292b93bcd6b
    configuration
      rom name=program.rom size=0x280000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Fatal Fury Special
    name:     Fatal Fury Special
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-3R-USA
    sha256:   410e90db3d38507ccc85ad3bca6b27a080123fd5160e82b5de4d914d4b6d6e61
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    FIFA International Soccer
    name:     FIFA International Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-84-USA
    sha256:   1f454f2ce16fb96ba0b950ceaa098fe6eabed9e4a0e512252debad8fa6bc5ef5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    FIFA Soccer '96
    name:     FIFA Soccer '96
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   M/SNS-A6SE-USA
    sha256:   26ff143c2f1a4a16ca838cc4b5555e39bbe7208515dca8f1c4b1a00dec61cf09
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x1e0000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    FIFA Soccer '97
    name:     FIFA Soccer '97
    region:   NA
    revision: 1.0
    board:    EA-1A3M-30/P10019
    serial:   SNS-A7IE-USA
    sha256:   c514f855ad377242582952eee95513a2152ebbf6bb5b06bdf9f031fc5ac748ab
    configuration
      rom name=program.rom size=0x1e0000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x280000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Fighter's History
    name:     Fighter's History
    region:   NA
    revision: 1.1
    board:    SHVC-BJ0N-01
    serial:   SNS-YH-USA
    sha256:   b9594d588816ae570ea5fea14577ed47de4db9ac9a40a116c84e0ad7a2ce58f8
    configuration
      rom name=program.rom size=0x280000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Final Fantasy: Mystic Quest
    name:     Final Fantasy - Mystic Quest
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-MQ-USA
    sha256:   6151389f33ce2e53db3cd99592440c0020f5f4668f581ce3bd615bc92077f255
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Final Fantasy II
    name:     Final Fantasy II
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-F4-USA
    sha256:   680535dc1c4196c53b40dc9c2c9bc159a77802ab8d4b474bef5dc0281c15ad06
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=BJ3M revision=10
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Final Fantasy III
    name:     Final Fantasy III
    region:   NA
    revision: 1.0
    board:    SHVC-BJ3M-10
    serial:   SNS-F6-USA
    sha256:   0f51b4fca41b7fd509e4b8f9d543151f68efa5e97b08493e4b2a0c06f5d8d5e2
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Final Fight
    name:     Final Fight
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-FT-USA
    sha256:   60cca2592d0756b8c4c7a0a254fafa5ac47aa752521fd1f77dcbf4b6ee1bee90
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Final Fight 2
    name:     Final Fight 2
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-01
    serial:   SNS-F2-USA
    sha256:   744d1a16c3f99970283597ae8f43b7c3621c5f995c4566ef24b8d70b0692007b
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Final Fight 3
    name:     Final Fight 3
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-AFZE-USA
    sha256:   f9dac709b2c2859f828103a0dd980716817e2bde3b9d7e2fdea36e9bb9bed7f2
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Final Fight Guy
    name:     Final Fight Guy
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-FY-USA
    sha256:   8c47f9dc79748c0ae6c648f8480614d22eaefade065612ad1fe749fc3db4d1bc
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Firepower 2000
    name:     Firepower 2000
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-FW-USA
    sha256:   003dba0193acc5336840307194643ca3b1f848dcfc77580b4e17c605105b27f5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Firestriker
    name:     Firestriker
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-3S-USA
    sha256:   6f32355bef68d4ad58822f50074b46bcc9a68357cd2c6a5470c96bf5344f84d8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    First Samurai
    name:     First Samurai
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-FK-USA
    sha256:   4c1354337efa788169387458fa6bdbcf4be0c98369920af2bd876ad98d29070f
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Flashback: The Quest for Identity
    name:     Flashback - The Quest for Identity
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-5F-USA
    sha256:   064a880a8dfcf576f74ae8a17c3ec7b0a27e8cb0300a5e5959452fcc30422f14
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Flintstones: The Movie
    name:     Flintstones - The Movie, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AFNE-USA
    sha256:   ff09d72d6241b331dc429d1cf27c04c26546f23312a22c7a14e6a4bf41ed1069
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Flintstones: The Treasure of Sierra Madrock
    name:     Flintstones - The Treasure of Sierra Madrock, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-9F-USA
    sha256:   3d5bbc06e7e9797d937c803610c40b262c14c3f0a39e8048dd6b0b052a040fc1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Football Fury
    name:     Football Fury
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-UF-USA
    sha256:   40b55ee44bc6f1c83daac3e1806dcf7ecd5b35280cdb6a63c7a5e52fbd2115e6
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x300000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Foreman For Real
    name:     Foreman For Real
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AFEE-USA
    sha256:   bc6b0344a434644c391ac69a53c7720c51e2d3c5c082b8d78598ae4df100c464
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Frank Thomas' Big Hurt Baseball
    name:     Frank Thomas' Big Hurt Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   SNS-AFKE-USA
    sha256:   ba93721bdb0f19506e722bc580d0b4dad6e8648dddbc52e3ce45dd75ea165f72
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Frantic Flea
    name:     Frantic Flea
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AF8E-USA
    sha256:   25b380f529f5a9295df7c0adcc5213d67f131f552d078a3d8f545f988047c90a
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Frogger
    name:     Frogger
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-AF7E-USA
    sha256:   1ce72767795f41049a1f4d2829e837a8885eb91e1c14faf1ca62d05839ebe3c9
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Full Throttle: All-American Racing
    name:     Full Throttle - All-American Racing
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AFTE-USA
    sha256:   3db8a496a49d47b1ac2a893feaf682722765b2cde63c22af3aa68212dcfa975f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    fun 'n games
    name:     Fun & Games
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-7N-USA
    sha256:   dcceb5be536c9b91bf34f65e7fcec4b55a78af0192323cf86f3b9555072037ee
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Gemfire
    name:     Gemfire
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-RL-USA
    sha256:   78e2b8210d8dd8776ee23153eb3dce2cbf7d8ddc3e8a5d25b580428f59d98bdb
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3B revision=01
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Genghis Khan II: Clan of the Gray Wolf
    name:     Genghis Khan II - Clan of the Gray Wolf
    region:   NA
    revision: 1.0
    board:    SHVC-1J3B-01
    serial:   SNS-6G-USA
    sha256:   05e9404bfe0689e484a34991f22712c9da718cf000b0f748378af5758b654a3d
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    George Foreman's KO Boxing
    name:     George Foreman's KO Boxing
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-GK-USA
    sha256:   fc993c122079b94ebc9b9452b46a55d5ddcd3715a4b97af795222b264827e90f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ghoul Patrol
    name:     Ghoul Patrol
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AGJE-USA
    sha256:   a4ceb31b82ea532e6eb640fa2eda61625758e72251efa5f0ae9a984f4a98a8a0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Goal!
    name:     Goal!
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-SU-USA
    sha256:   feff03cfa195512ad87dc876012d70520cf192e2874c518b6dbdf3d876ea60c4
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Gods
    name:     Gods
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-GZ-USA
    sha256:   8796ca4de3aeffd3a494fe28e7d7e2aeb220ca652b43684f29e2cc94f02c20c4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Goof Troop
    name:     Goof Troop
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-G6-USA
    sha256:   2bb368c47189ce813ad716eef16c01cd47685cb98e2c1cb35fa6f0173c97dd7c
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    GP-1
    name:     GP-1
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-G7-USA
    sha256:   342ddf438aec3f0696de8f2cd74bb7f48a956f488f1246eeccaff5ef246ca50b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    GP-1: Part II
    name:     GP-1 - Part II
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-11
    serial:   SNS-AGRE-USA
    sha256:   2e478569949c3935529c956448947ef90bae64abaf3d523bd89e7f4cf5e83702
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Gradius III
    name:     Gradius III
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-G3-USA
    sha256:   93da752a0c76167d0907efa832367e5d14aab8e835b864f345c386071a9af718
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    The Great Circus Mystery: Starring Micky & Minnie
    name:     Great Circus Mystery - Starring Micky & Minnie, The
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-11
    serial:   SNS-4C-USA
    sha256:   b5492aea296ee4bfcd2c677fbec5153fd4c4db758c835b372ef750cf2399649b
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Great Waldo Search
    name:     Great Waldo Search, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-GW-USA
    sha256:   3ab1ca181000f10cb7c2ae8dc9fafeecd5d77314ee92960e26dff0d6a1fd11ee
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    GunForce
    name:     GunForce
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-GU-USA
    sha256:   f921297c361f16ad3f1257e91829638fc795f9323172015d7237ed648c8f7515
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Hagane
    name:     Hagane
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AHGE-USA
    sha256:   5a6deb5617e86a9f4b981031e939510e30c5c8ad047f5f012e40442113fd28c2
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hal's Hole in One Golf
    name:     Hal's Hole in One Golf
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-JO-USA
    sha256:   de1cf1512e48473b03adb87b7504f394e8b330b346bac24044f833d83609799a
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hammer Lock Wrestling
    name:     Hammer Lock Wrestling
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LJ-USA
    sha256:   21cd6d749674c55342f5f1a895642d4b58b9bd7eb211f5df3a35dc6c2f5d4501
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Hard Ball III
    name:     Hard Ball III
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-3Y-USA
    sha256:   4fb9b010e4b1dc166ab7995a6f9491114063b68aac344004b1edfc555951d959
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Harley's Humongous Adventure
    name:     Harley's Humongous Adventure
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-HV-USA
    sha256:   6e2a02a8944c19db3da76d2646f232fbe3ed039bc7d44cc03910814fa77a7acf
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Harvest Moon
    name:     Harvest Moon
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   SNS-AYWE-USA
    sha256:   73a3aa87ddd5ce5df5ba51292316f42b6e128280179b0a1b11b4dcddc17d4163
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Head-On Soccer
    name:     Head-On Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AVSE-USA
    sha256:   9ff6bbcce7dc1f3bded96860a86698cab161d13ee167c57b5b114ac646eea0ea
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hit the Ice
    name:     Hit the Ice
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-HC-USA
    sha256:   fa418618787145291201b3d9247048b2d7dfba37f6556dcb1d40db499124dd60
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Home Alone
    name:     Home Alone
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-HA-USA
    sha256:   7c34908526db2a634216fab0276c42a8e4e22d59c728cd586200142a12dd2c2c
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Home Alone 2: Lost in New York
    name:     Home Alone 2 - Lost in New York
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-HN-USA
    sha256:   27eaccb4eea93832639565a664bf3561ed5a11ac025d377a3f33262d851c1b2b
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Home Improvement
    name:     Home Improvement
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-AHIE-USA
    sha256:   48a3ac52e2c9128abc2dc60e07817a51898e8a93be0d51d6f541a8635263a089
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hook
    name:     Hook
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-HO-USA
    sha256:   acad4c594f156e0f30dec2532b35fcb3bab800e08263377634e2d96dfd055f3e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Hunt for Red October
    name:     Hunt for Red October, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-RO-USA
    sha256:   a03528344d40bf800cdbde2dd240b30442cec7aea6026fbbe312a7c36bf0f74a
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hurricanes
    name:     Hurricanes
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AHUE-USA
    sha256:   41cc900d2461a8dc4706640e493885ddf85db04d8dffceebf7a0ed89cc983d8d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hyper V-Ball
    name:     Hyper V-Ball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-HQ-USA
    sha256:   db57f45e113127b7148c1b0add2da888e16e16b3e46749f95973b3ef497ae90b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Hyper Zone
    name:     Hyper Zone
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-HZ-USA
    sha256:   f57c49cc3e4c5e34929e12658db0de8794265c517e42f3c518bb1466ba46f14a
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    The Ignition Factor
    name:     Ignition Factor, The
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-AIFE-USA
    sha256:   fa143784fd20721d61101920e6aae9b7f5012b8157b1ce0c7ea83ea6c875d84d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J3B revision=01
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Illusion of Gaia
    name:     Illusion of Gaia
    region:   NA
    revision: 1.0
    board:    SHVC-1J3B-01
    serial:   SNS-JG-USA
    sha256:   32adc048fd89228a4310c03df243e83de7d436cdb2460b4cc83ade20d359b2bd
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Imperium
    name:     Imperium
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-DN-USA
    sha256:   4dc2b5de98e9109583d9b61b38d26a8216af4dba246ef71350122213630562d0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Incantation
    name:     Incantation
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AIYE-USA
    sha256:   c41150c0e84055801377fb7cb273cc51ca442b269ca6287cadf514f553e34750
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Incredible Crash Dummies
    name:     Incredible Crash Dummies, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-C7-USA
    sha256:   0415adfe80977485f84cdffaaa79f18c91c004c7dba202b4cf9a94eb11adeada
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Incredible Hulk
    name:     Incredible Hulk, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8U-USA
    sha256:   8d170628d2d2fdf142bb82ad80e908bba54c45fa33241c779b8eafaf1b21171f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Indiana Jones' Greatest Adventures
    name:     Indiana Jones' Greatest Adventures
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AIJE-USA
    sha256:   cf611b40f9570e8fcfdc21db623965da62561e8ca82ded59e432ba6fb0bfb562
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Inindo: Way of the Ninja
    name:     Inindo - Way of the Ninja
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-IN-USA
    sha256:   5e5cac64fdcf09a52a9c0ab1ebc8bd309dcb1256faf1405284443569b5284fe5
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=YJ0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Inspector Gadget
    name:     Inspector Gadget
    region:   NA
    revision: 1.0
    board:    SHVC-YJ0N-01
    serial:   SNS-5G-USA
    sha256:   c7b1706a0ee96f3e0d65cd043c05966bfe3d5c57d08bbd2df3118817424adf82
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    International Superstar Soccer
    name:     International Superstar Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-3U-USA
    sha256:   6443fecebcdc2ff7061a40432f3ad1db6dfd354909a5f306972cf6afa122752c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    International Superstar Soccer Deluxe
    name:     International Superstar Soccer Deluxe
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AWJE-USA
    sha256:   d2fe66c1ce66c65ce14e478c94be2e616f9e2cad374b5783a6a64d3c1a99cfa9
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    International Tennis Tour
    name:     International Tennis Tour
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-IT-USA
    sha256:   4e8044b1e7a779685d6751351eef2391272ac8b2bd909edeecfc3d3c5a594bef
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Irem Skins Game
    name:     Irem Skins Game, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-MT-USA
    sha256:   8304d8bc55aa9e64bdd144d384f4b185af2426e7d64888c6c23dd41366a53981
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Itchy & Scratchy Game
    name:     Itchy & Scratchy Game, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AISE-USA
    sha256:   39bfe828571cdb23e7c85c23cf5b175979dcc2042c5841add58f5ae6492168a9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Izzy's Quest for the Olympic Rings
    name:     Izzy's Quest for the Olympic Rings
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AIZE-USA
    sha256:   183af7d642b55d52cd594786ec2f031d033cc6c8c1a2a84a834e4ada04301b26
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jack Nicklaus Golf
    name:     Jack Nicklaus Golf
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-JN-USA
    sha256:   3ffbb6e0ccf8e9f5e4c72d9fe526a16371e562b71a91d6430e562bf358a5126b
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    James Bond Jr.
    name:     James Bond Jr.
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-JJ-USA
    sha256:   9cf82dd9a851cdc38bf2afc286c077ff18a0a5d3bb301eba606cc52db62f8965
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jammit
    name:     Jammit
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-J6-USA
    sha256:   8b7b33f3d2edb844f1d43cfdd595b8c2cb6fc005d59acb899a1afda999e47638
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jeopardy!
    name:     Jeopardy!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-JY-USA
    sha256:   85e5f6fedc420925557092d728e1dc6b4e2042368fb78bf93c0df447f8c9c0c0
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jeopardy! Deluxe Edition
    name:     Jeopardy! Deluxe Edition
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-7J-USA
    sha256:   76d760da59aa7fc2fd4426c1d1aec57412703ab901b9df60ac2be16ede80b0e7
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jeopardy! Sports Edition
    name:     Jeopardy! Sports Edition
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8Y-USA
    sha256:   d7110ddc0b34aa4ecd5e55f7b846657edb982db82cf0ba340fe0464daf0ca9be
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Jetsons: Invasion of the Planet Pirates
    name:     Jetsons - Invasion of the Planet Pirates, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8J-USA
    sha256:   ee7a29eb9c302ea2bb235ef990fd8344a6beb9817499941c40a8a94ad5f7c964
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jim Lee's Wild Covert Action Teams
    name:     Jim Lee's Wild Covert Action Teams
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AWIE-USA
    sha256:   a314583b11d594b8245b5177aa64a4d3b7497d096edabbea7c1842c57aa2ad2b
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jim Power: The Lost Dimension in 3D
    name:     Jim Power - The Lost Dimension in 3D
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-6J-USA
    sha256:   6f0bec87ece503b0fbe108cd159ed6f5fa7711b1c4fe31e982af41ad5c638093
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jimmy Connors' Pro Tennis Tour
    name:     Jimmy Connors' Pro Tennis Tour
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-JC-USA
    sha256:   205890f94e27e7c1b381124cc35c866351bafa906383e507cd17ccb9d1b68ffd
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Jimmy Houston's Bass Tournament, U.S.A.
    name:     Jimmy Houston's Bass Tournament, USA
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   M/SNS-AFUE-USA
    sha256:   6abe908b4005a68d1f6e9a54339c94a33db41801890d8c058ce974fdab3d0bcd
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Joe & Mac
    name:     Joe & Mac
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-JT-USA
    sha256:   3b2b8b813b58049a4a86ce1c42d2a651f19fd9bbab2407a494e20cf343d3c1a4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Joe & Mac 2: Lost in the Tropics
    name:     Joe & Mac 2 - Lost in the Tropics
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-J3-USA
    sha256:   c9889799a9ae8558f26d66ae546db930c99acb78d921b4395afbbc38da6272aa
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Judge Dredd: The Movie
    name:     Judge Dredd - The Movie
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AJDE-USA
    sha256:   67fa7115eb6bf292c024c3a8b06ce9bd457c4d46de182a06a573afff968cc0f1
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Jungle Book
    name:     Jungle Book, The
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-7K-USA
    sha256:   771a0322d9f5f8e13a52d01e80257a1f75cc693cf4abf74793520eb5f8b5580e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jungle Strike
    name:     Jungle Strike
    region:   NA
    revision: 1.0
    board:    MJSC-1A0N-30-2
    serial:   M/SNS-AJGE-USA
    sha256:   8d812ea4fa897274116f7f43bc689e110f1cfbd3f7eb3a1737c2a85d36369159
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jurassic Park
    name:     Jurassic Park
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-J8-USA
    sha256:   fe91d45201753ae9655d5ce38838e352f478b26b2d933c1bcb5bd8330121f9ff
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jurassic Park
    name:     Jurassic Park
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30-1
    serial:   SNS-J8-USA
    sha256:   0a4e9d6fa2ac16aa51da5538d93280734de480e44c430173ed14826c84553c7d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Jurassic Park 2: The Chaos Continues
    name:     Jurassic Park 2 - The Chaos Continues
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-A2JE-USA
    sha256:   5eff7c27f69b3ebc1ec1294ffcd1debf3512bc3e6c1c75fbdc5e778dcaaba1c9
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BA0N revision=01,10
    rom name=program.rom size=0x280000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Justice League Task Force
    name:     Justice League Task Force
    region:   NA
    revision: 1.0
    board:    SHVC-BA0N-10
    serial:   M/SNS-AJLE-USA
    sha256:   7f05959f423bc656091ea3bddfbc89c877ae243dca346f63233e27973f34e0eb
    configuration
      rom name=program.rom size=0x280000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ka-Blooey
    name:     Ka-Blooey
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-BB-USA
    sha256:   05152bcf9bf086e7bcdbfa7dd8edfe2085f1339c4d7e193e0071c49a10471ef4
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Kawasaki Caribbean Challenge
    name:     Kawasaki Caribbean Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-10
    serial:   SNS-KC-USA
    sha256:   7cc3693cc5e1e834d57795f04b048fab27864a898a9507e7ca383771e2035414
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Kawasaki Superbike Challenge
    name:     Kawasaki Superbike Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AKEE-USA
    sha256:   fec6dd097e378e795dd164be658b10b1c60672b2351a7f4a7722d1fe5736410e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Ken Griffey Jr. Presents Major League Baseball
    name:     Ken Griffey Jr. Presents Major League Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-20
    serial:   SNS-JR-USA
    sha256:   3104d6c06c8909c56f6adb2faecf1b4382f2490370798b605631da926c5306d8
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Ken Griffey Jr. Presents Major League Baseball
    name:     Ken Griffey Jr. Presents Major League Baseball
    region:   NA
    revision: 1.1
    board:    SHVC-1A3M-21
    serial:   SNS-JR-USA
    sha256:   f9f7a2de8cbafd6f9042841dfc42529f8245d75f436bed1c23f9ba1663182e61
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x400000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Ken Griffey Jr.'s Winning Run
    name:     Ken Griffey Jr.'s Winning Run
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-A9GE-USA
    sha256:   18b5276be764c06531219c1757d40214aeec06fae68f4ce3ec78b58ee750a43e
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Kendo Rage
    name:     Kendo Rage
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-M7-USA
    sha256:   b16661d5151d73cea5ac46d7c899531c7b2cdee2558092c23a5460c8092b80b8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Kid Klown in Crazy Chase
    name:     Kid Klown in Crazy Chase
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ZI-USA
    sha256:   914652f72d6ceb83b8b43414d6c42581ec12e9b3f45259b8b2768d26b8d4f073
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Killer Instinct
    name:     Killer Instinct
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-AKLE-USA
    sha256:   618a23636e07110e094277ec1d1e60c3620a6e9a5f386292808267593fa803ad
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x280000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    King Arthur & The Knights of Justice
    name:     King Arthur & The Knights of Justice
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-20
    serial:   M/SNS-AAKE-USA
    sha256:   1b58d0aed4510811c73d267244a7e915aa0b334c86e68f3fa5883f5cb534e4d7
    configuration
      rom name=program.rom size=0x280000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    King Arthur's World
    name:     King Arthur's World
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-RC-USA
    sha256:   aca9eb59d6783e2cae3787c69053888fea98f5dfe4c8af8b5a6360e0afb3b5d7
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    King of Dragons
    name:     King of Dragons
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-EI-USA
    sha256:   6638b5541059814d4c34574e5e277ef613aebf81c91d3def557a7642fb5840e1
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    King of the Monsters
    name:     King of the Monsters
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-KM-USA
    sha256:   1135858a1ce686a0a163bb0e6f3a4d7cd71c0cd56efbc79677372f2624cf2306
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    King of the Monsters 2
    name:     King of the Monsters 2
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-KT-USA
    sha256:   a9729d049ce580839bbfef1836a13dc16f2fc934d203ebf7390e0b1c47ea9a2d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1L3B revision=02,11
    sa1
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x2000
      ram size=0x800
      map id=io address=00-3f,80-bf:2200-23ff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=bwram address=00-3f,80-bf:6000-7fff
      map id=bwram address=40-4f:0000-ffff
      map id=iram address=00-3f,80-bf:3000-37ff
  information
    title:    Kirby Super Star
    name:     Kirby Super Star
    region:   NA
    revision: 1.0
    board:    SHVC-1L3B-11
    serial:   SNS-AKFE-USA
    sha256:   4e095fbbdec4a16b075d7140385ff68b259870ca9e3357f076dfff7f3d1c4a62
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x2000
      ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Kirby's Avalanche
    name:     Kirby's Avalanche
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-PQ-USA
    sha256:   67937dd7a29a93b1aaabb6df89f0748369ff47f3f6c655a402c00d5657973140
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Kirby's Dream Course
    name:     Kirby's Dream Course
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   SNS-CG-USA
    sha256:   0f984dc5fe8293f75e3b8fad98b0cb564706d9b1e3902b56415aa399c2d4df2b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1L5B revision=11,20
    sa1
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x8000
      ram size=0x800
      map id=io address=00-3f,80-bf:2200-23ff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=bwram address=00-3f,80-bf:6000-7fff
      map id=bwram address=40-4f:0000-ffff
      map id=iram address=00-3f,80-bf:3000-37ff
  information
    title:    Kirby's Dream Land 3
    name:     Kirby's Dream Land 3
    region:   NA
    revision: 1.0
    board:    SHVC-1L5B-20
    serial:   SNS-AFJE-USA
    sha256:   b50bf9d95485e8aeb7a6730e9f9f9c9c4ec23a85b336a4fb2e3b63034531e36f
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x8000
      ram size=0x800

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Knights of the Round
    name:     Knights of the Round
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-LO-USA
    sha256:   fb601ead645edce139b0483d3155b4e3d7ab245bf87a3a66cb88c0a617c0a526
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Krusty's Super Fun House
    name:     Krusty's Super Fun House
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-FH-USA
    sha256:   bed18c968aee0eb0c866c1964c28135364cd6d65fff7bcb5873342c04e63750d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Krusty's Super Fun House
    name:     Krusty's Super Fun House
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-02
    serial:   SNS-FH-USA
    sha256:   e36322697c48baae6492db91e6cbf3844a420f6e0cc8a75f3a73556026ddbbb8
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Kyle Petty's No Fear Racing
    name:     Kyle Petty's No Fear Racing
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AKPE-USA
    sha256:   daf3e45bafbec81ffa5911b94810b2cd267574de717292360f9940f41fb2a6a9
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Lagoon
    name:     Lagoon
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-LA-USA
    sha256:   46c811f0cacffe8f20e1d63072d25d7c47e9bb3fd5124851fd05aca9884d21fb
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Lamborghini American Challenge
    name:     Lamborghini American Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-L8-USA
    sha256:   48cd9476fef1ed685b9c30dd1669b46048f7295cbbb2abcfa5b1a48699346ea3
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Last Action Hero
    name:     Last Action Hero
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-L5-USA
    sha256:   314d53f409b66ba3f4426a6f1bb7c69f6779aeed277ce2e19535f94d7c8ca586
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Lawnmower Man
    name:     Lawnmower Man, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-LW-USA
    sha256:   a179a1188220b59787c49a78a0dde79b89380e3a8a8a0ab558f0102c5796f873
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Legend
    name:     Legend
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6L-USA
    sha256:   c7814cee0fc95d6422cf19a3dc8c9a65b60f6f56da75f09cebea02cc5f99261b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Legend of the Mystical Ninja
    name:     Legend of the Mystical Ninja, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-GG-USA
    sha256:   c865fb17e8c59a21d32b9a605319241fa74ec732e3f0ee58f5d7fcbd8ee57c6b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    The Legend of Zelda: A Link to the Past
    name:     Legend of Zelda - A Link to the Past, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-ZL-USA
    sha256:   66871d66be19ad2c34c927d6b14cd8eb6fc3181965b6e517cb361f7316009cfb
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    La Legende de Zelda: La Triforce des Dieux
    name:     Legende de Zelda - La Triforce des Dieux, La
    region:   QC
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-ZF-CAN
    sha256:   dd499445275fca6692c0487a8bd70a23f6c8e78e766df0e3c58fbbe53f8adb06
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Lemmings
    name:     Lemmings
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-LE-USA
    sha256:   4b28d2ddab405976bb56e41a40ec1ea11d7362a8f398d5f8c117d715a15719ca
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Lemmings
    name:     Lemmings
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-10
    serial:   SNS-LE-USA
    sha256:   cd016c41c7ef9f4f243d57c2b1564b4ceb3b4c38cc165cd02ab6c8e35c93aa2e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Lemmings 2: The Tribes
    name:     Lemmings 2 - The Tribes
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-L2-USA
    sha256:   63ecdca7f89b2432ccd57bdb81c0875f6336353f5897f756ef162ab2ec2ee707
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Lester the Unlikely
    name:     Lester the Unlikely
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-01
    serial:   SNS-LY-USA
    sha256:   a2c1970670e2831e47e24ced01bf4ba5aba05cac3773bf524c62d689c35687e1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Lethal Enforcers
    name:     Lethal Enforcers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LK-USA
    sha256:   3bc5f296c3dbee012e93a5cf25568f9288ce87b34d74085401a560350eaca03f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Lethal Weapon
    name:     Lethal Weapon
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-L3-USA
    sha256:   80c22cc92d51a54de9cd9fd00db5ff58a35fff35e822169c94e445d50834fba3
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x140000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Liberty or Death
    name:     Liberty or Death
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-01
    serial:   SNS-7L-USA
    sha256:   8fcb9c34aea863d49ff08a0ace3f83d301b81f01e2ede28bee7e6d778878d0cc
    configuration
      rom name=program.rom size=0x140000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    The Lion King
    name:     Lion King, The
    region:   NA
    revision: 1.0
    board:    MJSC-1J0N-20-2
    serial:   SNS-ALKE-USA
    sha256:   457abe634e0a8be03e29513a3dca8f3e9d0ddc6bf97d8931f2316094260f3712
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1K0N revision=01
    rom name=program.rom size=0x80000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram size=0x200
      map id=io address=00-1f,80-9f:6000-7fff select=0x1000
  information
    title:    Lock On
    name:     Lock On
    region:   NA
    revision: 1.0
    board:    SHVC-1K0N-01
    serial:   SNS-AZ-USA
    sha256:   0e2ba574ff73587f211c8f818d444631584832e9240548f003171c11b898ad62
    configuration
      rom name=program.rom size=0x80000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram name=dsp1.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Looney Tunes B-Ball
    name:     Looney Tunes B-Ball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ALTE-USA
    sha256:   76ba0fc1f5c1f39354bb3173a600f23915f1191f400f7d525d220b4b3c8d958d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Lord of the Rings: Volume 1
    name:     Lord of the Rings - Volume 1, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-64-USA
    sha256:   62557ee2a3fc3b5a3f59431f966eb61bb380ba983ef6c7742cb55cf075f15f6c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Lost Vikings
    name:     Lost Vikings
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LV-USA-1
    sha256:   9f7782a92fda789f9d119b1f0a2f7da0f35606357556a48eca9487797ee1a888
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Lost Vikings 2
    name:     Lost Vikings 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ALVE-USA
    sha256:   ab3d97c1a3a979e1680a428ec65df54cfb72997bbfe2173292248a4fa8c51ba1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Lufia & The Fortress of Doom
    name:     Lufia & The Fortress of Doom
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-ES-USA
    sha256:   73731a5a7932965de02a9e98055dcf88b4d17b8f710a6ecfde3e36a1f248773b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x280000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Lufia II: Rise of the Sinistrals
    name:     Lufia II - Rise of the Sinistrals
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   M/SNS-ANIE-USA
    sha256:   7c34ecb16c10f551120ed7b86cfbc947042f479b52ee74bb3c40e92fdd192b3a
    configuration
      rom name=program.rom size=0x280000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    John Madden Football
    name:     Madden NFL '92
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-JM-USA
    sha256:   5a76347672ea7d27bb334b1e30bbc73e06f92373883bed499245377327a8f0cf
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    John Madden Football '93
    name:     Madden NFL '93
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-MF-USA-1
    sha256:   3e62872bf69ea90dd7093608268f8defa2c6016adb1011745dab3c2af45d69b7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Madden NFL '94
    name:     Madden NFL '94
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-01
    serial:   SNS-9M-USA
    sha256:   7e77e196db47e87a5b297e60f0dfa7ce41df8d2d1fdd9152e06628d0b0e586af
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Madden NFL '95
    name:     Madden NFL '95
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-11
    serial:   SNS-ANLE-USA
    sha256:   0ad77ae7af231313e1369a52d1622b88e3751aa5ec774628df7071f9e4244abc
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Madden NFL '96
    name:     Madden NFL '96
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-A6FE-USA
    sha256:   3059d86cdc383985c564a7a891fe18e08f5222ead7ede9fa309159d60cde13a1
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Madden NFL '97
    name:     Madden NFL '97
    region:   NA
    revision: 1.0
    board:    EA-1J3M-20/P10018
    serial:   SNS-A7NE-USA
    sha256:   6874568d985f65dd817d4b03998e71c8cbacc8d8707411fde7bffee350605a88
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Madden NFL '98
    name:     Madden NFL '98
    region:   NA
    revision: 1.0
    board:    EA-1J3M-20/P10018
    serial:   SNS-A8NE-USA
    sha256:   e3c62c9fe55d2311aa6a264f41b45d6cbc7b1b069ed3aa82ee57d381c062547d
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Magic Boy
    name:     Magic Boy
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-YG-USA
    sha256:   c01fb8989d391d3e343003934937f02bd8ef9aacdad68c32c3d3f56feb72f5b0
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Magic Sword
    name:     Magic Sword
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-MD-USA
    sha256:   1d3cceaa05e054b002caeb09fd5fb9e718ec446764f4169d97bc185da76fdf4d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Magical Quest Starring Mickey Mouse
    name:     Magical Quest Starring Mickey Mouse, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-MI-USA
    sha256:   f301bb8ea867e530ecb64e8eff504ed5b9697cf076c70e2036ecf2ffbe6c487a
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mario is Missing!
    name:     Mario is Missing!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-MU-USA
    sha256:   8267e2f092c86d5a29c9a826db82c7473638e28e2507cdaf5c86981f07cd0bef
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Mario Paint
    name:     Mario Paint
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-20
    serial:   SNS-MP-USA-1
    sha256:   e842cac1a4301be196f1e137fbd1a16866d5c913f24dbca313f4dd8bd7472f45
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mario's Early Years: Fun with Letters
    name:     Mario's Early Years - Fun with Letters
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-AMYE-USA
    sha256:   22b21fb39d40447846f6ff77a07f7b4aba2a7473941ba50c787aae6153b1fb5e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mario's Early Years: Fun with Numbers
    name:     Mario's Early Years - Fun with Numbers
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-YR-USA
    sha256:   57b94f5f576abbe8bd8f5a4eeb6cf927300ec1b5f0596610f3539ba733505c12
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mario's Early Years: Preschool Fun
    name:     Mario's Early Years - Preschool Fun
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-AMEE-USA
    sha256:   c646b1e60e2cd5a2741915f5d3dfe3c17f45ff7283f8052e840bd4354b0990e1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mario's Time Machine
    name:     Mario's Time Machine
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8M-USA
    sha256:   4b69d4958e099c3b3f6ae45e153ced9b24755d8c161dfee06c9f67886a7c0f09
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Mark Davis' The Fishing Master
    name:     Mark Davis' The Fishing Master
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-AOAE-USA
    sha256:   908440f96fd0df14602fc6d1daee8519fc31f765ad00bf64aaba35c2c6ef0b56
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Marvel Super Heroes in War of the Gems
    name:     Marvel Super Heroes in War of the Gems
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AHZE-USA
    sha256:   49dd77b310b476c875633335243553be59ecfb0bffae62e46f2e53ff05c20fcd
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mary Shelley's Frankenstein
    name:     Mary Shelley's Frankenstein
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AFRE-USA
    sha256:   2731f0bd1c87e75121f41d1ed5cc9fbf177f414b8bf831c76fd9c4b58c86ed08
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Mask
    name:     Mask, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AMGE-USA
    sha256:   44cc113ce1e7616cc737adea9e8f140436c9f1c3fba57e8e9db48025d4ace632
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Math Blaster: Episode One
    name:     Math Blaster - Episode One
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AMME-USA
    sha256:   22453182d44380f08004a12c6492a0c4b2e1f584e268dcc3296a03ea03ae0909
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x300000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Maui Mallard in Cold Shadow
    name:     Maui Mallard in Cold Shadow
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AZBE-USA
    sha256:   a203a13870eaec92095daef1196a0c9fe8416e600504d55dd0dc724d4f5f5cb0
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Mecarobot Golf
    name:     Mecarobot Golf
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-TS-USA
    sha256:   c3c4afdeb67136619c643bd9a9f3fe90337541a40745503b5d4eb9b9e6e64b67
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    MechWarrior
    name:     MechWarrior
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-06
    serial:   SNS-WM-USA
    sha256:   2a08704748f5ef6488348c4099729feca600412d331bda3756e51efd8b94e113
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    MechWarrior 3050
    name:     MechWarrior 3050
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-A35E-USA
    sha256:   7bffa1dc31604fa3d61e06ce2c59168098cc8dd7e59998e1d5f30c49bdf8d617
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mega Man 7
    name:     Mega Man 7
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-A7RE-USA
    sha256:   a255fec32453739903a1954149f19bc9658f4a415600b44badf1d4e5e13a16f9
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mega Man Soccer
    name:     Mega Man Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-RQ-USA
    sha256:   cf4d603dc0a3759da571224c671a9bfd29f9e52ca8dbb61bcc8ac8be5481e9b2
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mega Man X
    name:     Mega Man X
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-01
    serial:   SNS-RX-USA
    sha256:   3e1209f473bff8cd4bcbf71d071e7f8df17a2d564e9a5c4c427ee8198cebb615
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=2DC0N revision=01
    hitachidsp model=HG51B169 frequency=20000000
      rom name=program.rom size=0x180000
      ram size=0
      rom name=cx4.data.rom size=0xc00
      ram size=0xc00
      map id=io address=00-3f,80-bf:6000-7fff
      map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
      map id=ram address=70-77:0000-7fff
  information
    title:    Mega Man X2
    name:     Mega Man X2
    region:   NA
    revision: 1.0
    board:    SHVC-2DC0N-01
    serial:   SNS-ARXE-USA
    sha256:   da484f2a636b8d692840f40e5468e992c5f2af26d365c69fbc12ef2923588d23
    configuration
      rom name=program.rom size=0x180000
      rom name=cx4.data.rom size=0xc00
      ram name=cx4.data.ram size=0xc00

release
  cartridge region=NTSC
    board type=1DC0N revision=01
    hitachidsp model=HG51B169 frequency=20000000
      rom name=program.rom size=0x200000
      ram size=0
      rom name=cx4.data.rom size=0xc00
      ram size=0xc00
      map id=io address=00-3f,80-bf:6000-7fff
      map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
      map id=ram address=70-77:0000-7fff
  information
    title:    Mega Man X3
    name:     Mega Man X3
    region:   NA
    revision: 1.0
    board:    SHVC-1DC0N-01
    serial:   SNS-AR3E-USA
    sha256:   b2aa2c0a621dfbed3b528de93282fc91abb16325d358680d34753d43629263cf
    configuration
      rom name=program.rom size=0x200000
      rom name=cx4.data.rom size=0xc00
      ram name=cx4.data.ram size=0xc00

release
  cartridge region=NTSC
    board type=2E3M revision=01
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
    obc1
      ram name=save.ram size=0x2000
      map id=io address=00-3f,80-bf:6000-7fff
      map id=io address=70-71,f0-f1:6000-7fff,e000-ffff
  information
    title:    Metal Combat: Falcon's Revenge
    name:     Metal Combat - Falcon's Revenge
    region:   NA
    revision: 1.0
    board:    SHVC-2E3M-01
    serial:   SNS-KD-USA
    sha256:   d4f2cb6b209db29f7aec62e5a23846681c14665fb007e94d7bcfc7b5611e938b
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Metal Marines
    name:     Metal Marines
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-01
    serial:   SNS-6M-USA
    sha256:   0a9609a505dd1555006b16f53d961b3ce50c518aa1597a77dcd46e55ecc716ff
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Metal Morph
    name:     Metal Morph
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-AMHE-USA
    sha256:   057484558ebd18165f98e556b994080535e31cefdd98b5edb190516f7040fc9d
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Metal Warriors
    name:     Metal Warriors
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AWME-USA
    sha256:   0d7f875877fe856066cfb39b4ecdbbe7d48393a75770720876c94419f809bb1c
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1K0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram size=0x200
      map id=io address=00-1f,80-9f:6000-7fff select=0x1000
  information
    title:    Michael Andretti's Indy Car Challenge
    name:     Michael Andretti's Indy Car Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1K0N-01
    serial:   SNS-AMAE-USA
    sha256:   88545d24c60c18c695cc3fce8c4074f46ad1bac905e8a39a61d8a6ae6b608621
    configuration
      rom name=program.rom size=0x100000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram name=dsp1.data.ram size=0x200

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x140000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Michael Jordan: Chaos in the Windy City
    name:     Michael Jordan - Chaos in the Windy City
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-11
    serial:   SNS-AWCE-USA
    sha256:   1b425ea5a883b7464637b74c2937fde699ffff52b53ad6940a66285e0663194a
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mickey Mania: The Timeless Adventures of Mickey Mouse
    name:     Mickey Mania - The Timeless Adventures of Mickey Mouse
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AMIE-USA
    sha256:   0773eb741ce28f963f767fc7dd44678eb3d37ed4dc7fc82bb9cce7d55f1cfc64
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mickey's Ultimate Challenge
    name:     Mickey's Ultimate Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6U-USA
    sha256:   add082caffe707df2c0259489c3e272d6557ab07ba3ff856cbc0adba0d7db6a5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Micro Machines
    name:     Micro Machines
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AH3E-USA
    sha256:   a2adeb4bf0e7cc943611ac726e5578da404373a79e91436c9bbd15480688b15c
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Might & Magic III: Isles of Terra
    name:     Might & Magic III - Isles of Terra
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   SNS-3H-USA
    sha256:   835ded9479f0e7babae00405d85233d767e82fa247aa1a9cdc14fd1f147b62ef
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mighty Morphin Power Rangers
    name:     Mighty Morphin Power Rangers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-52-USA
    sha256:   624a66607caef2ca34920ea15b84b28cdd1916ee089d496cec4f1d43621fdbb3
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mighty Morphin Power Rangers: The Fighting Edition
    name:     Mighty Morphin Power Rangers - The Fighting Edition
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A3RE-USA
    sha256:   1b85c0690aa156a255c7f79e133e453345452698fa98abf8df744c262d0cf865
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mighty Morphin Power Rangers: The Movie
    name:     Mighty Morphin Power Rangers - The Movie
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-20
    serial:   M/SNS-A2RE-USA
    sha256:   c706b70097c679f6f0ced6f77a30620807d0b2555fc3c683c0ec2fc791176039
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Miracle Piano Teaching System
    name:     Miracle Piano Teaching System, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-MR-USA
    sha256:   8546dfc91f6256df8b21083531457e3edf0021da12fce6858e2c59ff239c31da
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    MLBPA Baseball
    name:     MLBPA Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XH-USA
    sha256:   d32f194e27d177e64bf3bda046889b8198276fca2e8772e4b02a17152e9273e4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mohawk & Headphone Jack
    name:     Mohawk & Headphone Jack
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AJYE-USA
    sha256:   8715a641f2e4dd8b6066be7f2683d9129fff3fcccaf0a09cc8bdd2aa56460764
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Monopoly
    name:     Monopoly
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-ML-USA
    sha256:   480ae7186fd5b28200cd88e136b9cd3b6600d32508e280a0bc27ea0ed8d3c0bb
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Monopoly
    name:     Monopoly
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30-2
    serial:   SNS-ML-USA
    sha256:   6b0ac4d52d24536cdb7d9d0dc7d19ee30d08ac34363983290c5912ccc850fa0d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mortal Kombat
    name:     Mortal Kombat
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-10
    serial:   SNS-KX-USA
    sha256:   3c6d3e4a9c9af160f1c1cf11ce4ead531d9500c1f58f1cbe682c90a5eaa3efb2
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mortal Kombat II
    name:     Mortal Kombat 2
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-28-USA
    sha256:   43e36a74fb73a7efc46b380599e269b1fff8f55ecf80f5cf50c34d02ceda041a
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mortal Kombat II
    name:     Mortal Kombat 2
    region:   NA
    revision: 1.1
    board:    SHVC-BJ0N-01
    serial:   SNS-28-USA
    sha256:   ca2f86ca77f822fcd8e86f5a287f2a76d0becbb81a7bce73ae22909beb2f834c
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Mortal Kombat 3
    name:     Mortal Kombat 3
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-A3ME-USA
    sha256:   417874aa57856fe93eefdb24066fa1a9ca3f23c72c09d5247ae2b3ab4b3d09d1
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x40000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mr. Do!
    name:     Mr. Do!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AUNE-USA
    sha256:   340293c06536d7b6981ad7c681e404f4390ff6c62340f844a4558877c1b82af0
    configuration
      rom name=program.rom size=0x40000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mr. Nutz
    name:     Mr. Nutz
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-N8-USA
    sha256:   3472dd574b50aed2fa998f464398db4fbb00f5a300a672c3737ee9336a008a16
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x40000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ms. Pac-Man
    name:     Ms. Pac-Man
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AN8E-USA
    sha256:   007735e68a91cab403f1c955d9d562e9311124e660fa5b32e5c5d0a2e052160e
    configuration
      rom name=program.rom size=0x40000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Musya: The Classic Japanese Tale of Horror
    name:     Musya - The Classic Japanese Tale of Horror
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-MY-USA
    sha256:   f292598ac462fdfcd32ad9b6b35ac01d4bab020391dff92bfe94780ec604289a
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Mutant Chronicles: Doom Troopers
    name:     Mutant Chronicles - Doom Troopers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AM9E-USA
    sha256:   8d4ada4f98464d176ae7f0fb8a20032056680f3241637a0f903f23f31f41ff36
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Natsume Championship Wrestling
    name:     Natsume Championship Wrestling
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-7W-USA
    sha256:   22d2260de02552b1205aac2ff5a202a4c80532ac28045ef5a058d88279ab758e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    NBA All-Star Challenge
    name:     NBA All-Star Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-NB-USA
    sha256:   c70b812a9d2df7f95b279e4050e03a4b8a68588a370816e645f378296b84e5d1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    NBA Give 'n Go
    name:     NBA Give & Go
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ANJE-USA
    sha256:   5874b942f974bb906d8cbc33b601a1faf8f14aee8d0995124c8dc84bb4973808
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    NBA Hang Time
    name:     NBA Hang Time
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   SNS-AXGE-USA
    sha256:   af2fe0627de2bb2672f4f65dcbdaaee22b211275f679f123d5fa5d37fd699363
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    NBA Jam
    name:     NBA Jam
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8N-USA
    sha256:   0f18c496426bb97fe5e8b91ad5299f0b1c3898ac17047b745c86b167c212ab7a
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BA1M revision=01
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NBA Jam: Tournament Edition
    name:     NBA Jam - Tournament Edition
    region:   NA
    revision: 1.0
    board:    SHVC-BA1M-01
    serial:   M/SNS-AJTE-USA
    sha256:   b257cffb3484e6be051a56268cb99ee888bd6d3e9c0e8d6d0779ff66c411f6ba
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NBA Live '95
    name:     NBA Live '95
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   SNS-ANBE-USA
    sha256:   2115c39f0580ce19885b5459ad708eaa80cc80fabfe5a9325ec2280a5bcd7870
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NBA Live '96
    name:     NBA Live '96
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   M/SNS-A6BE-USA
    sha256:   2d6fc4214245684a8f8f9bb553de637b7c660919ec775bfe3baaf74060c9157e
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NBA Live '97
    name:     NBA Live '97
    region:   NA
    revision: 1.0
    board:    EA-1A3M-30/P10019
    serial:   SNS-A7LE-USA
    sha256:   4b945493d28653d5d047a375136ec9792f27e652be4af00e2c03c40369bd6ecf
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NBA Live '98
    name:     NBA Live '98
    region:   NA
    revision: 1.0
    board:    EA-1A3M-30/P10019
    serial:   SNS-A8LE-USA
    sha256:   1d1b257dcf859cc412d3a9521fc58dc876a5917e6a69cd1d960a8e9840454bb4
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NBA Showdown
    name:     NBA Showdown
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-10
    serial:   SNS-6N-USA
    sha256:   6a7324734004d99206439430243b51a05fa8c25ffa314dafc7f127235d1a730f
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    NCAA Basketball
    name:     NCAA Basketball
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-06
    serial:   SNS-DU-USA
    sha256:   351587366eca8aeb38236e4ad6cbd4a1b6e37b8cc592725a79249c3a054fa3a7
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    NCAA Basketball
    name:     NCAA Basketball
    region:   NA
    revision: 1.1
    board:    SHVC-1A1B-06
    serial:   SNS-DU-USA
    sha256:   8ef5d5c50ffeca1e62e88e4fe2909eaf191e28fbb5a9faf98b7b10bea72c9ed9
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NCAA Final Four Basketball
    name:     NCAA Final Four Basketball
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   SNS-AFIE-USA
    sha256:   d297c76efbcd9a528d8ee51025f8774ab2cb6bbc676c24e28592c50f47e71bfc
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NCAA Football
    name:     NCAA Football
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-AFBE-USA
    sha256:   8c43b7c2f2ef1fca9237c64c2f4c9e98d1a48940dae500ce8eac71278d34efb3
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Newman-Haas IndyCar featuring Nigel Mansell
    name:     Newman-Haas IndyCar featuring Nigel Mansell
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ANME-USA
    sha256:   489d6d704d3d917d9b46ce382230700419983a8d0d61a98fe07e08c935522dde
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    NFL Football
    name:     NFL Football
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-NF-USA
    sha256:   f651a42bb351f7691b880827178c36fcf6c265c7833c6de1d94f7ed69bac0517
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NFL Quarterback Club
    name:     NFL Quarterback Club '95
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-Q9-USA
    sha256:   6a59115a9958d4a9fa167095505a2ddf222ca6291209d07618319e39a2be8b61
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NFL Quarterback Club '96
    name:     NFL Quarterback Club '96
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-20
    serial:   SNS-AQBE-USA
    sha256:   f43f8ec546b8060e9d191fca860c38caf5a43eda86a304f0073647c6fad7b2c9
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    NHL '94
    name:     NHL '94
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-4H-USA
    sha256:   5132e1c0d466963e6adc09e8a608ebd90619ab94f7fc908d626bbaf6a99dfa19
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    NHL '95
    name:     NHL '95
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-11
    serial:   SNS-ANHE-USA
    sha256:   01c0b58d5fd5d5484fea988455a55a71ed9e606538d2b3ce3f216159cc6929b0
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    NHL '96
    name:     NHL '96
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-A6HE-USA
    sha256:   d24c0175ee4eafed88e277691c5f5dafd4e197723097e2eb68aa6b40f449fff2
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    NHL '97
    name:     NHL '97
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-AH7E-USA
    sha256:   2a2dc2ef84efd9a773d1e8231b7e3e57f0de7e4528968670963f2f1f358eef39
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J5M revision=11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    NHL '98
    name:     NHL '98
    region:   NA
    revision: 1.0
    board:    SHVC-1J5M-20
    serial:   SNS-A8HE-USA
    sha256:   8113c2cedafc8fd5a56c8638ae340fb275f263ff5c5e18d04dc6c3ebc5cfffee
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    NHL Stanley Cup
    name:     NHL Stanley Cup
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-01
    serial:   SNS-NH-USA
    sha256:   d44f487d84f5bb761955b7b70a5464b2f094e199875f595f312c88e04ac647ff
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    NHLPA Hockey '93
    name:     NHLPA Hockey '93
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-HY-USA-1
    sha256:   55f3432a130085c112d65aa6443c41eb7a8aeec59aad2c2b4b2ac536b604b449
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Nickelodeon GUTS
    name:     Nickelodeon GUTS
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-ANGE-USA
    sha256:   d7ad6f67860da78fe25d9e79dd13af7ac7efaa0c8e0103898a4849ab4af9e438
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Nigel Mansell's World Championship Racing
    name:     Nigel Mansell's World Championship Racing
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-M8-USA
    sha256:   ce9c819d6496e58901b39d9b04558a89e09ccc3aac33690b8d02bb0406682a57
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ninja Gaiden Trilogy
    name:     Ninja Gaiden Trilogy
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ANRE-USA
    sha256:   fccc96af24a2463b1c53253e1c5c8ef63641355fae53c0fb410427f29743262b
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ninja Warriors
    name:     Ninja Warriors
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-NI-USA
    sha256:   3c109e50b91ec6df3bb8509778ae544c99433fb40dda9b801178dfe513053618
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    No Escape
    name:     No Escape
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ANOE-USA
    sha256:   f099937ac4c8afb38c517c5d85475224985fb8f345dacb44994a617ea05bf4e5
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Nobunga's Ambition
    name:     Nobunga's Ambition
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-01
    serial:   SNS-NZ-USA
    sha256:   437ea48711d54c2275286170a17948cb57ba9d961ba475715c0dba5fbd61a2db
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Nobunga's Ambition: Lord of Darkness
    name:     Nobunga's Ambition - Lord of Darkness
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-11
    serial:   SNS-IZ-USA
    sha256:   ae572a51ebe429cfc06a3b75d7db9581f2884f9adc78827dc162b4c4ddc6ce2d
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Nolan Ryan's Baseball
    name:     Nolan Ryan's Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-NR-USA
    sha256:   7f3d0ebac6ecfb99cfd1d5b13210e989df9e8b2f2319a63c42faef8ad115a964
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Nosferatu
    name:     Nosferatu
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-NS-USA
    sha256:   9712829b38f23229d4e3d65da78237659c790235f425c6b12487e4d9e9a37ae9
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Obitus
    name:     Obitus
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-7B-USA
    sha256:   31bc862ab0a8eabf23b5124e13940cb3501e7ecdd3f15e34142248ceb4aa139a
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Ogre Battle: The March of the Black Queen
    name:     Ogre Battle - The March of the Black Queen
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   M/SNS-OB-USA
    sha256:   b766c26498d0afd63f44aefdef42642d2483b54f18d2b81a4f1d67a57f641044
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Olympic Summer Games
    name:     Olympic Summer Games
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AO9E-USA
    sha256:   a137f4fc1c635f706d10e0c7927e71f52da171ce2d27394ce0deb451b5bed8ae
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    On the Ball
    name:     On the Ball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-CT-USA
    sha256:   e153195de7b59dd5b9854952cccca6bb93164e5fdff8292124bee6bbe5dbf16f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J5M revision=01
    rom name=program.rom size=0x140000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,90-9f:6000-7fff mask=0xe000
    map id=ram address=30-3f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Operation Europe: Path to Victory 1939-45
    name:     Operation Europe - Path to Victory 1939-45
    region:   NA
    revision: 1.0
    board:    SHVC-2J5M-01
    serial:   SNS-YP-USA
    sha256:   9e1a39aaf3585af0d6a5d79de3c35fcfe030c34bd4b09606a6fb8c7def19340b
    configuration
      rom name=program.rom size=0x140000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Operation Logic Bomb
    name:     Operation Logic Bomb
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-IY-USA
    sha256:   190742792a950a112f893cba0e083eb787cf24293f698967defff929635ba0e7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x140000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Operation Thunderbolt
    name:     Operation Thunderbolt
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-11
    serial:   SNS-36-USA
    sha256:   5cbed0401734142184166917427d24f9e5f107a7adea665e2f4b4101491ad54b
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Oscar
    name:     Oscar
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AOZE-USA
    sha256:   0c08e6b817e4d0b333acb910a0bde3d79bd2dc188defc5df9a7c1233fa81c98d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Out of This World
    name:     Out of This World
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-TW-USA
    sha256:   582548dc86598a3557e9e3c27285c81964b006a954affe5c73948da5375ea11c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Out to Lunch
    name:     Out to Lunch
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-P8-USA
    sha256:   54b2f03393109ac7fd36d8c7752f15a44d9607ab0187a371b853191db3592c01
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Outlander
    name:     Outlander
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LD-USA
    sha256:   db44f8b58a31b640a47aa4390101c3c6a5f613e4e49c636d44786278033dec19
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pac-Attack
    name:     Pac-Attack
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-P9-USA
    sha256:   10c8abce67b49f8afbe880d2f13e0fd6d5efc162df34d5941e4a94851f23b2ff
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pac-in-Time
    name:     Pac-in-Time
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-APTE-USA
    sha256:   7fe4cb9c294d66589ff78e225774471ecb7db80df25f2b6199ca25671358072b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pac-Man 2: The New Adventures
    name:     Pac-Man 2 - The New Adventures
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-25-USA
    sha256:   4cb52ba751c42d9e12ca429e5d657622a370b608002880a997f64de453f0de20
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=2A5M revision=01
    rom name=program.rom size=0x140000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    P.T.O.: Pacific Theater of Operations
    name:     Pacific Theater of Operations
    region:   NA
    revision: 1.0
    board:    SHVC-2A5M-01
    serial:   SNS-TK-USA
    sha256:   83727f6954bce024edac7f3fd18a6fbf63d05b580f692d96faa37e60893c91cd
    configuration
      rom name=program.rom size=0x140000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1J5M revision=11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    P.T.O. II: Pacific Theater of Operations
    name:     Pacific Theater of Operations II
    region:   NA
    revision: 1.0
    board:    SHVC-1J5M-20
    serial:   M/SNS-ATEE-USA
    sha256:   2c6119fbe40d23576adde568d60ffae793bbf6b512f8fea8dcd0b1cd2623ef02
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Packy & Marlon
    name:     Packy & Marlon
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-APYE-USA
    sha256:   b1586c8ece4da9697f0602a684d7a9108027247a34652c3771831d31f82ee078
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Pagemaster
    name:     Pagemaster, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-APME-USA
    sha256:   ecaf51852e111512385c5f209d51578e65b45fcaa17b295ca06120c8d1464520
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Paladin's Quest
    name:     Paladin's Quest
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-01
    serial:   SNS-LN-USA
    sha256:   46286d0839a4397fc4c067b39783f98d2aefeca870a468bae601a1434f1dde90
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Paperboy 2
    name:     Paperboy 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-P2-USA
    sha256:   7cec4ffc3eda0441561717cf55927901b5fbbd669c254079f78ca74c67c4a17b
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    The Peace Keepers
    name:     Peace Keepers, The
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-R6-USA
    sha256:   c414a4084b3d03aba19496d2efdd68fcf826194d8f1308f5c98e3a7af2fcc063
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1L3B revision=02,11
    sa1
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000
      ram size=0x800
      map id=io address=00-3f,80-bf:2200-23ff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=bwram address=00-3f,80-bf:6000-7fff
      map id=bwram address=40-4f:0000-ffff
      map id=iram address=00-3f,80-bf:3000-37ff
  information
    title:    PGA European Tour
    name:     PGA European Tour
    region:   NA
    revision: 1.0
    board:    SHVC-1L3B-11
    serial:   SNS-AEPE-USA
    sha256:   5abfb974ca0e56aabb3f4126817d14a546c57a5a5c6042d31196063d80996698
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000
      ram size=0x800

release
  cartridge region=NTSC
    board type=1L3B revision=02,11
    sa1
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000
      ram size=0x800
      map id=io address=00-3f,80-bf:2200-23ff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=bwram address=00-3f,80-bf:6000-7fff
      map id=bwram address=40-4f:0000-ffff
      map id=iram address=00-3f,80-bf:3000-37ff
  information
    title:    PGA Tour '96
    name:     PGA Tour '96
    region:   NA
    revision: 1.0
    board:    SHVC-1L3B-02
    serial:   SNS-A3GE-USA
    sha256:   571d3129350bfb7cca2285499cf31dda48d1047cf9eaef122c8c33dffa9ad296
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000
      ram name=internal.ram size=0x800

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    PGA Tour Golf
    name:     PGA Tour Golf
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-PG-USA
    sha256:   5c0b5266a191852ca593235f07180e673cb79e3f0b0dd31f65808eef83bf6e90
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Phalanx: The Enforce Fighter A-144
    name:     Phalanx - The Enforce Fighter A-144
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-PH-USA
    sha256:   0663330bc061f4b768fa1806610878ef6e6cf546f36041ae087c8e55703693b8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Phantom 2040
    name:     Phantom 2040
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A24E-USA
    sha256:   b7291088f5c49e1fc55bf932076ec03f7b39f6e409ae06e884b57024c56cdc87
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Pieces
    name:     Pieces
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-Z5-USA
    sha256:   a0b39d7fd7c39c5b0f41f3542fb8d2887530ded1c111b4ffb2a863845e704ecc
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1B0N revision=02,03
    rom name=program.rom size=0x80000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram size=0x200
      map id=io address=30-3f,b0-bf:8000-ffff select=0x4000
  information
    title:    Pilotwings
    name:     Pilotwings
    region:   NA
    revision: 1.0
    board:    SHVC-1B0N-02
    serial:   SNS-PW-USA
    sha256:   724144e34990069213591b5df067dd437a613b30f669840e9098db1dce626d2d
    configuration
      rom name=program.rom size=0x80000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram name=dsp1.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pinball Dreams
    name:     Pinball Dreams
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-7D-USA
    sha256:   3a52bf09850aa054dca443f7ea74d43f201dffecc40326924ecba9b0f1450e43
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pinball Fantasies
    name:     Pinball Fantasies
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-APFE-USA
    sha256:   0888d20ab2f834c77b0a2dc2162c43890a1640adc78c6b0bb5892ca8d5008ad3
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pink Goes to Hollywood
    name:     Pink Goes to Hollywood
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-YW-USA
    sha256:   d0f4a5040ecf96dc49aa0084160e291a38f2ee75319750db4d6687ab36828da9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Pinocchio
    name:     Pinocchio
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-ACGE-USA
    sha256:   98c51c3bb577600fe79577c323333a791baa30904f37c695890e6e380b75e3c8
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Pirates of Dark Water
    name:     Pirates of Dark Water, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8P-USA
    sha256:   447dfa710e69479159e9d407474fbf5f67d3a3330ab0c7627afd123ded3fdb3a
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pit-Fighter
    name:     Pit-Fighter
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-PF-USA
    sha256:   c2a1a66648a0a0bfe2f201cf4f926d138e410fbf85ecf436ccb9aac70c0df3de
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pitfall: The Mayan Adventure
    name:     Pitfall - The Mayan Adventure
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-APAE-USA
    sha256:   e03d117d8b3093b0bbad5224638f85378b254b81eb304e506a732b4338802e0f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Plok
    name:     Plok
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-P4-USA
    sha256:   0b3b0fd42bbe06a812053df376b183412fc0de335c4b7cb8e45f3fe47b0044e9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pocky & Rocky
    name:     Pocky & Rocky
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-KK-USA
    sha256:   72b2b3bead3fcd27a1610ad5d4d8be3235efeaff96df2e7858911992a5892d21
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pocky & Rocky 2
    name:     Pocky & Rocky 2
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-29-USA
    sha256:   cc33ae02114ea18a86592de327b2b4bcc80728b11a5e4c61666dca71480d4169
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Populous
    name:     Populous
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-PO-USA
    sha256:   5e580f220ed16281df8ee9a5f450b553f39f8c4078d3f3048d66bda15f98e19f
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Porky Pig's Haunted Holiday
    name:     Porky Pig's Haunted Holiday
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-KX-USA
    sha256:   ee9759fdb590ba908f569c2bb8a63703d282b58b84bd1fe0a472ea47685acdc5
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x280000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Power Instinct
    name:     Power Instinct
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-20
    serial:   SNS-AGKE-USA
    sha256:   06c8fc466805f97c9147711b2d8370d4f4d05d9fa3a916f17aa1682f73c9a63b
    configuration
      rom name=program.rom size=0x280000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Power Moves
    name:     Power Moves
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-P3-USA
    sha256:   42ee7829d3db0213b718168c29674879bb4532573e9fb3450a5b417174a16ed0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Power Piggs of the Dark Age
    name:     Power Piggs of the Dark Age
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AOTE-USA
    sha256:   0288ec049723cd0c7f1148cdc1aef0b6922b8a756affe373c99d5690e0dfceaa
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1L3B revision=02,11
    sa1
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000
      ram size=0x800
      map id=io address=00-3f,80-bf:2200-23ff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=bwram address=00-3f,80-bf:6000-7fff
      map id=bwram address=40-4f:0000-ffff
      map id=iram address=00-3f,80-bf:3000-37ff
  information
    title:    Power Rangers Zeo: Battle Racers
    name:     Power Rangers Zero - Battle Racers
    region:   NA
    revision: 1.0
    board:    SHVC-1L3B-11
    serial:   M/SNS-A4RE-USA
    sha256:   2e2a8c0da7f6def617077a25b222062f6988ef14b36b2edfe10d47c6a942d023
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000
      ram name=internal.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Prehistorik Man
    name:     Prehistorik Man
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-APUE-USA
    sha256:   8f387d083de1399bb79e5312c31a6f1757f2a536bfa25cecf1aea77bfd77058b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Primal Rage
    name:     Primal Rage
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AR9E-USA
    sha256:   55376715f243b1bacd9aeecf1092bbc7837fe512592a2c1703d24b0829fc1934
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Prince of Persia
    name:     Prince of Persia
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-PR-USA
    sha256:   494190cd6d7fd68882cbe255a6e237d9c4bdaf3988615ede0297a5e285ad5dd9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Prince of Persia 2: The Shadow and the Flame
    name:     Prince of Persia 2 - The Shadow and the Flame
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AUPE-USA
    sha256:   04ca1a481093c4a7e12f18b33697d6e05e50e30e0f5b1655aa265abd14719bba
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Pro Quarterback
    name:     Pro Quarterback
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-QB-USA
    sha256:   85e2e58e1f99fce366c85d49f77446395584ca4959ef772a707fe454ed46c68f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Pro Sport Hockey
    name:     Pro Sport Hockey
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-UI-USA
    sha256:   00b5b3d3febd1a6f28ceb49e39cdd9476a944fe62ea3850808cdeafaaaa63040
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Push-Over
    name:     Push-Over
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-PV-USA
    sha256:   475c9baa1c2b76a6b3119e47d32814dc1c08e84e23250ae015bb0bccea915637
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Q*bert 3
    name:     Q-bert 3
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-Q3-USA
    sha256:   89d57bf308033ef17f770a80080cbeed2d112244635d5b5f860f2016398cd2f6
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Race Drivin'
    name:     Race Drivin'
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-RV-USA
    sha256:   dd0feb78e2d5d81f59241baf3bca5e2edaebbe98f0ac860a4eb6d448718f1ca5
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Radical Psycho Machine Racing
    name:     Radical Psycho Machine Racing
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-05
    serial:   SNS-RP-USA
    sha256:   edf990e502c646a2fe83fcd1d359ca0ed5003ace06cb4c3de5a51a0c56d6ec54
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Radical Rex
    name:     Radical Rex
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ARRE-USA
    sha256:   1869c0faf93bf21b7ff965f1925fad4b2924a64b1e48f4837221eebdf741226c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Raiden Trad
    name:     Raiden Trad
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-RD-USA
    sha256:   5fd7666e509f9d3cf1fd6b209dc22f2f3848f74eae7b83239d1090e031fc6df2
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rampart
    name:     Rampart
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-RM-USA
    sha256:   e19f7d8d5c3e4cefeff5889380d8780495e01f0553d13be4182a15a5d4b615bb
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Ranma 1/2: Hard Battle
    name:     Ranma Half - Hard Battle
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-01
    serial:   SNS-R2-USA
    sha256:   32d32ef56af83887cdc2c04b3da4be1cd82a473988deaa2e7dd50d38ef79c3a1
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rap Jam: Volume One
    name:     Rap Jam - Volume One
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ARVE-USA
    sha256:   920bbaaf6a32cab5feabb5bc8b2b427dccd53bfd31d0da8acb7ea4e819139e4f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Realm
    name:     Realm
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ARQE-USA
    sha256:   097cbe9720903bc14599158b80e0cc314ef2fe8a585d6d0a8962eb1626031492
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Red Line: F-1 Racer
    name:     Red Line - F1 Racer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6R-USA
    sha256:   549f2e5b17f685cad25ba71ce7bc6e004e7bfd09e6be12a827af9a9a26556fff
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Relief Pitcher
    name:     Relief Pitcher
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-5R-USA
    sha256:   b2221d0a59a399e437710e1191a37071d0a59c72d8e62427cd499de8d8fd7b61
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Ren & Stimpy Show: Buckeroo$!
    name:     Ren & Stimpy Show - Buckeroo$!, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ARBE-USA
    sha256:   d322ce440076be0e3678277596acee8089098f4397b35ac8b3df88be5ce5e02f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Ren & Stimpy Show: Fire Dogs
    name:     Ren & Stimpy Show - Fire Dogs, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6Y-USA
    sha256:   71e7083cfcf32b738f60f5eeffd4f9d1fd9250afbde0c56e22a4b97abac377a1
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x140000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Ren & Stimpy Show: Time Warp
    name:     Ren & Stimpy Show - Time Warp, The
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-ARTE-USA
    sha256:   ad7dd4efb8836d4009f6c76bd21865d8f5dcf9c3cbd8fa7bb32d686488847120
    configuration
      rom name=program.rom size=0x140000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Ren & Stimpy Show: Veediots!
    name:     Ren & Stimpy Show - Veediots!, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-V8-USA
    sha256:   ba54d715abf100b94fee801351986fa818e4309730cefbacf9b4fad36e284c1c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Revolution X: Music is the Weapon
    name:     Revolution X - Music is the Weapon
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-20
    serial:   M/SNS-AXRE-USA
    sha256:   5fb072c3c2e9d8e7f84bea9c4bf2253e6868eb2b1f13e35a7d75fdf05896d873
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rex Ronan: Experimental Surgeon
    name:     Rex Ronan - Experimental Surgeon
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XR-USA
    sha256:   f44482e2cccd9fcfd5875d84ff700f6e783f3bd8abd1ac4d939074cd6ad3fe65
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Riddick Bowe Boxing
    name:     Riddick Bowe Boxing
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-01
    serial:   SNS-XG-USA
    sha256:   e3ea89964ee82d2ae1e5739b8503acf62732aedef28d1b3d5caa9ebae0feec06
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Rise of the Phoenix
    name:     Rise of the Phoenix
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-11
    serial:   SNS-QG-USA
    sha256:   cce455b7074f62ffdb41863ee8c160ea06f7acd028837666329bc1e0c3567ad0
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Rise of the Robots
    name:     Rise of the Robots
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-AROE-USA
    sha256:   38be8013bbe07b2020ba30031fb0a2c77bad8a3eb61fac8217adfe82d6c402af
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rival Turf!
    name:     Rival Turf!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-RB-USA
    sha256:   3f59cc687d22cd1b23cc33ae6e4518234c9da813c01f79f4c43716e12d32a12d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Road Riot 4WD
    name:     Road Riot 4WD
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-RR-USA
    sha256:   864aa9068fb23cd20022a9ac36fb9082299278ea0cb07a20deec2b6a1c6cbc70
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Road Runner's Death Valley Rally
    name:     Road Runner's Death Valley Rally
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-DV-USA
    sha256:   2e8203e421f97cf165f03a5d4f69dadf0bcca18c42c6a1dfe79c8705c522cc54
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    RoboCop 3
    name:     RoboCop 3
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-R3-USA
    sha256:   055d9c6311a663af7c899a6f76a419c274c57baada3ef64c52fadb1c676b1446
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    RoboCop versus The Terminator
    name:     RoboCop vs. The Terminator
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-VR-USA
    sha256:   a2115e7576dec06e0de613efb89de861815a78ef72e78a3784be09fb7541928f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Robotrek
    name:     Robotrek
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-11
    serial:   SNS-E9-USA
    sha256:   1e2ded7b1e350449b7a99b7ec414525e4b9b086c416deeee5eb3e48e032c46bd
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rock n' Roll Racing
    name:     Rock & Roll Racing
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-RN-USA
    sha256:   9d721753301278325c851f1843d669a697aed757dcf6495a31fc31ddf664b182
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Rocketeer
    name:     Rocketeer, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-RK-USA
    sha256:   b072fd9b08042e3262446fdf418a41848251072a32bd7f8335cc03543c4ae6c8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rocko's Modern Life: Spunky's Dangerous Day
    name:     Rocko's Modern Life - Spunky's Dangerous Day
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-8D-USA
    sha256:   cde8876b99c337ff932322506ceef05519e5882b853c54bb8c650d9500cd5957
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Rocky Rodent
    name:     Rocky Rodent
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-NP-USA
    sha256:   7c0f915b581796e5b6dd384ecdc0dad8af4d956492fbcedec628c8845d911d7e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Roger Clemens' MVP Baseball
    name:     Roger Clemens' MVP Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-VP-USA
    sha256:   e21300c8c4170e084fd83ce4c842dd73f939fbd10ddfe47c9a49b6e291dcd52d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Roger Clemens' MVP Baseball
    name:     Roger Clemens' MVP Baseball
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-20
    serial:   SNS-VP-USA
    sha256:   f7e3c3012af2dbad350646b6ef3470f0b4c42e4a2873109f7aa6c81d7157c887
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A5B revision=02,04
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Romance of the Three Kingdoms II
    name:     Romance of the Three Kingdoms II
    region:   NA
    revision: 1.0
    board:    SHVC-1A5B-04
    serial:   SNS-XL-USA
    sha256:   815bfcf4fd6eb23a20c2e50dde023c210b273ffb6cd86a93909d803c3643ce46
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=2J5M revision=01
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,90-9f:6000-7fff mask=0xe000
    map id=ram address=30-3f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Romance of the Three Kingdoms III: Dragon of Destiny
    name:     Romance of the Three Kingdoms III - Dragon of Destiny
    region:   NA
    revision: 1.0
    board:    SHVC-2J5M-01
    serial:   SNS-S3-USA
    sha256:   88fa0a78ca98c6386e086c7fa9e81a2625bdecc55115dde6c6f4770b0670aa40
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1J5M revision=11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Romance of the Three Kingdoms IV: Wall of Fire
    name:     Romance of the Three Kingdoms IV - Wall of Fire
    region:   NA
    revision: 1.0
    board:    SHVC-1J5M-20
    serial:   M/SNS-AS4E-USA
    sha256:   9670c67eeeb5581fa5345025a2e2ee875179e0da9f5be4bf78641e477d785f17
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    R-Type III: The Third Lightning
    name:     R-Type III - The Third Lightning
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ER-USA
    sha256:   4d6c7d6d2693d8d43bafaff7582f9a94885362dadd9ee4012bbbdce1ba10c30e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Run Saber
    name:     Run Saber
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-RU-USA
    sha256:   4158e3e8890a52f0b12dc9ad5a29276058a247ff41e9f1d22897ebde1eb11269
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Samurai Shodown
    name:     Samurai Shodown
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-A7SE-USA
    sha256:   5db804171fca42486485ed85e4afe45b29e6d01304bdf75d520bfc42429739e3
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Saturday Night Slam Masters
    name:     Saturday Night Slam Masters
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-ZW-USA
    sha256:   34e1af0642c85148c5a3dc3c7ab4bcbda13a9fea190934b5526c555fff035651
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Scooby-Doo Mystery
    name:     Scooby-Doo Mystery
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AXDE-USA
    sha256:   7fb5236d10852125f0f37c2188b907d636647400a57bccbdb2f63098ffae8b2d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    SeaQuest DSV
    name:     SeaQuest DSV
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ASQE-USA
    sha256:   a4ab8cfad2f236675b1c0124f8484688e149f38e8628a3b38e9ec14d491ec07e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Secret of Evermore
    name:     Secret of Evermore
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-AEOE-USA
    sha256:   17c864a76d498feb6479eee8e7d6807b951c66225033228622bb66754baab1db
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Secret of Mana
    name:     Secret of Mana
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-01
    serial:   SNS-K2-USA
    sha256:   4c15013131351e694e05f22e38bb1b3e4031dedac77ec75abecebe8520d82d5f
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Shadowrun
    name:     Shadowrun
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-10
    serial:   SNS-WR-USA
    sha256:   e6bc0a595d5c7c4bc0bbb61ffe35a70288a77eb78544ed74682d489a9e6f07f4
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Shanghai II: Dragon's Eye
    name:     Shanghai II - Dragon's Eye
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-DE-USA
    sha256:   bdbcc53b266159b5a08e307e5b60fdb0cb5a1ba8f8c1c6c7f89d81eaf5133e85
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Shaq-Fu
    name:     Shaq-Fu
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-AQFE-USA
    sha256:   c73757eea258e169e506eaef989227a59918060f94117917f338183db14c50b6
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Shien's Revenge
    name:     Shien's Revenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-OO-USA
    sha256:   454c580498a7d317ed61cb5863b1999eff57ad440ecd30ebb76e193c9c52f3ac
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1J5M revision=11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Sid Meier's Civilization
    name:     Sid Meier's Civilization
    region:   NA
    revision: 1.0
    board:    SHVC-1J5M-20
    serial:   SNS-EQ-USA
    sha256:   de2d5a952096c5f50368b9270d342aa6e7a39007ffbec27117e182e30ef4cf32
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Side Pocket
    name:     Side Pocket
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-4P-USA
    sha256:   d09ca5adaee65cfd686742482bc55b1a3ce9bc5ebed61f24c5631555151a7fc7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    SimAnt: The Electronic Ant Colony
    name:     SimAnt - The Electronic Ant Colony
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-01
    serial:   SNS-AN-USA
    sha256:   c0bd1b378337c32047a6b7122a3813beb646e496fbdb1fa5c87ab9856271e4c5
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A5B revision=02,04
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x8000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    SimCity
    name:     SimCity
    region:   NA
    revision: 1.0
    board:    SHVC-1A5B-02
    serial:   SNS-SC-USA
    sha256:   e9c0bc05511e05a0d7c3e7cc42e761e1e8e532d46f59b9854b6902e1a2e9dd0a
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1J5M revision=11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    SimCity 2000
    name:     SimCity 2000
    region:   NA
    revision: 1.0
    board:    SHVC-1J5M-20
    serial:   M/SNS-AWWE-USA
    sha256:   bf74c58e4190faca2f3a967dc190fe529d13887d1262b72e057b5353e43cf67f
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A3B revision=20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    SimEarth: The Living Planet
    name:     SimEarth - The Living Planet
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-20
    serial:   SNS-SE-USA
    sha256:   446a1036d036986fdea7906c83832d3ba79ef63a6ed8c4e88b89ab9cb25daded
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Simpsons: Bart's Nightmare
    name:     Simpsons - Bart's Nightmare, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-BN-USA
    sha256:   f0d98e9061d0f6a193bb856de8a592f336dada97c41966e8d03119ba97465413
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Simpsons: Virtual Bart
    name:     Simpsons - Virtual Bart, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AVBE-USA
    sha256:   29c28188234ddbb0b72fc84253dcd3514e23034779c773db8097b073b73390c8
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    S.O.S.: Sink or Swim
    name:     Sink or Swim
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-9J-USA
    sha256:   70008efe51185eb0a2f8d8d8ac2bdbb99bd3dfcc169dcc474962f82692998051
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Skuljagger: Revolt of the Westicans
    name:     Skuljagger - Revolt of the Westicans
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-SL-USA
    sha256:   e10070f01845505ae8bfdf7b5b492e7209c2ae876f169fb6ff420dea269f4da3
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Skyblazer
    name:     Skyblazer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LZ-USA
    sha256:   a4ba1483db79c3f6278082387bce216d8f3e3b11ca32d49516d27f5ac07135a5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Smart Ball
    name:     Smart Ball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-JB-USA
    sha256:   cbca00fa5dfd6c72db2f21d010255657c33f7ac48de2554262035ead11bdf314
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Snow White in Happily Ever After
    name:     Snow White in Happily Ever After
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ASHE-USA
    sha256:   f653b8308b617e1cba1caf004e5a670088b882e320fa6afcc7a29dd3b2dd0ea0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Soldiers of Fortune
    name:     Soldiers of Fortune
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-01
    serial:   SNS-UD-USA
    sha256:   6fe7c8d39fcfab7f0a18e837a7ee0dd162e0557d6989c6e0d10c81616d3a0b8b
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Sonic Blast Man
    name:     Sonic Blast Man
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-SK-USA
    sha256:   75a7b5b8ad0329dc828d3201089e125fd55fdfc99d4cec704ffcd7e3036c2410
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Sonic Blast Man II
    name:     Sonic Blast Man II
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-11
    serial:   SNS-2C-USA
    sha256:   efe78f6fc68ddd0f6ef0ad9e0223d9417c14fcadece987dc8f50423fd6723b27
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    SOS
    name:     SOS
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-TT-USA
    sha256:   8f0a7670fe53d30f721784e5fff78349eb157a9f0eb2246206f9d7db478b7c56
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Soul Blazer
    name:     Soul Blazer
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-SO-USA
    sha256:   8438da09de8ce9aded3bb08644543f7b60fb60cffc68ce2d67d6a0643f2ecfc2
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Space Ace
    name:     Space Ace
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-5A-USA
    sha256:   85887dfa92374048fb20809c00eabea428992024cf875d287d0431b9767cc7cb
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Space Football: One on One
    name:     Space Football - One on One
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-FL-USA
    sha256:   24f3f22949f36ebf8ab4beaa8cba22db107efe7a7585f749343f2860bf041fe1
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x40000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Space Invaders
    name:     Space Invaders
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-IC-USA
    sha256:   dc5353ddc350816619230f25f8c51bddabf7438e6dfba21662eb1c4794856735
    configuration
      rom name=program.rom size=0x40000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Space Megaforce
    name:     Space Megaforce
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AT-USA
    sha256:   f5b7418c00ccac44615cfc57c7e17d57533837056886f6d733e6b714c36dec1f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spanky's Quest
    name:     Spanky's Quest
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-HJ-USA
    sha256:   14dc44687c8da35aec63b9edadbaac21bf7293f5171646f614139192e82ab071
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Sparkster
    name:     Sparkster
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ASSE-USA
    sha256:   eaa06470734ea57eff9b888137aa468fcb7bb149a0870a85e68c9db123de4670
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Spawn
    name:     Spawn
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-A9WE-USA
    sha256:   32d0f1ca5b91fd9b2caf81422fb9e8fb30bc091f0b2a429b9269dd307fcba4fd
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0xe0000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spectre
    name:     Spectre
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-7Q-USA
    sha256:   22d0b7687d66436abaf447bb29558e43f562283ec4dbe744df7d79a3e27a2577
    configuration
      rom name=program.rom size=0xe0000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Speed Racer in My Most Dangerous Adventures
    name:     Speed Racer in My Most Dangerous Adventures
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-9S-USA
    sha256:   54046ec1fc57f6165ad33080502f72809d1b06b438a5a0d0a80ffa2bb3df8b0b
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Speedy Gonzales: Los Gatos Bandidos
    name:     Speedy Gonzales - Los Gatos Bandidos
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ASSE-USA
    sha256:   df02d0f4f40e2732138309d38e91b48aef482490979007ecb63359a35115dfd4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Speedy Gonzales: Los Gatos Bandidos
    name:     Speedy Gonzales - Los Gatos Bandidos
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30-2
    serial:   SNS-ASEE-USA
    sha256:   68a51b7a06b6a9e7100a89521e52b5c467c46c828c0f6504bee677beac2aa6fd
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spider-Man: The Animated Series
    name:     Spider-Man - The Animated Series
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ADME-USA
    sha256:   f05d777e3de69aab18d336cac0af07f794f8d00090d085f86cebaed3679cabad
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spider-Man & Venom: Maximum Carnage
    name:     Spider-Man & Venom - Maximum Carnage
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AMCE-USA
    sha256:   964d21996e385e032b5d18baf716692ba1db780245cd71956c212045c1b8eb9a
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x300000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spider-Man & Venom: Separation Anxiety
    name:     Spider-Man & Venom - Separation Anxiety
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-A2CE-USA
    sha256:   b72fbbfe737eff49f59dcef9f13b963e50c5bc322d7eb0e7b4c25f3a71aa0815
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spider-Man & X-Men: Arcade's Revenge
    name:     Spider-Man & X-Men - Arcade's Revenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-MN-USA
    sha256:   63210a91573fa8e19592f2e6c746a400831d804c00453739447d2df32e731df7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Spindizzy Worlds
    name:     Spindizzy Worlds
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-SX-USA
    sha256:   fe10238ae42ed9eb4d906a81dd50ebe585140982cdfe266308ce1f16e78e6903
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J3M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    The Sporting News Baseball
    name:     Sporting News Baseball, The
    region:   NA
    revision: 1.0
    board:    SHVC-1J3M-20
    serial:   M/SNS-AWBE-USA
    sha256:   8d3f20af0be588cf2edc39aee0d96d4085512919a05c0e90a7848e414cc20e42
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Sports Illustrated: Championship Football & Baseball
    name:     Sports Illustrated - Championship Football & Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LU-USA
    sha256:   254d17a82a32d8bd231ca3a87d356b65e65cb0229902a69a57c21a4c99bbba1f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1C0N5S revision=01
    superfx revision=1
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000
      map id=io address=00-3f,80-bf:3000-34ff
      map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
      map id=ram address=60-7d,e0-ff:0000-ffff
  information
    title:    Star Fox
    name:     Star Fox
    region:   NA
    revision: 1.0
    board:    SHVC-1C0N5S-01
    serial:   SNS-FO-USA
    sha256:   3857b5294ea8f7468849437bb2d8271564e8a0ff30774622e9c872bcbd53a84d
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x8000
      superfx revision=1

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Star Trek: Deep Space Nine - Crossroads of Time
    name:     Star Trek - Deep Space Nine - Crossroads of Time
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A9DE-USA
    sha256:   3a16ad45ae3d89b13c9e53e21c2a4c725ff7cec7fbe7896d538d163f92cb4aac
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Star Trek: Starfleet Academy - Starship Bridge Simulator
    name:     Star Trek - Starfleet Academy - Starship Bridge Simulator
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ASTE-USA
    sha256:   b4d0b4a3fd73469f5469dfcc2316e50848ebf0630a225df2969c740759e321f4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Star Trek: The Next Generation - Future's Past
    name:     Star Trek - The Next Generation - Future's Past
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XN-USA
    sha256:   22c907b56ac6f414365801ed375c3fbf75696ce7f34ec89e1724628dc5622957
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Stargate
    name:     Stargate
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AGTE-USA
    sha256:   9ffd95486904804aca1f7068c99f1c9e91c2c0e5474ec758df1a913393571cc7
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Steel Talons
    name:     Steel Talons
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-5S-USA
    sha256:   91f938b4989215b1cd39635797f23b59b9d7b6d36e583f9eb69d022abe537bfc
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Sterling Sharpe: End 2 End
    name:     Sterling Sharpe - End 2 End
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AS2E-USA
    sha256:   e3b07a59f969ced91c4579bb459f2c747a6c3f12c45ae4776483ef4830f5f00f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Stone Protectors
    name:     Stone Protectors
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ASOE-USA
    sha256:   dad9c116283322d5a13fd659874c681582abdff3df182cc4c90511d33fb7110a
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Street Combat
    name:     Street Combat
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-RA-USA
    sha256:   b4626cf0c876a124b50f9421c48a7d762e9ed808ad336c799d543d60b484897c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1N0N revision=01
    sdd1
      rom name=program.rom size=0x400000
      map id=io address=00-3f,80-bf:4800-4807
      map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
      map id=rom address=c0-ff:0000-ffff
  information
    title:    Street Fighter Alpha 2
    name:     Street Fighter Alpha 2
    region:   NA
    revision: 1.0
    board:    SHVC-1N0N-01
    serial:   SNS-AUZE-USA
    sha256:   910a29f834199c63c22beddc749baba746da9922196a553255deade59f4fc127
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Street Fighter II: The World Warrior
    name:     Street Fighter II - The World Warrior
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-S2-USA
    sha256:   2b34161e96ef3f0f48cecd67e531a9bb94310652d8686f301bac426e4ab97e77
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=3J0N revision=01
    rom name=program.rom size=0x280000
    map id=rom address=00-2f,80-af:8000-ffff
    map id=rom address=40-6f,c0-ef:0000-ffff
  information
    title:    Street Fighter II Turbo: Hyper Fighting
    name:     Street Fighter II Turbo - Hyper Fighting
    region:   NA
    revision: 1.0
    board:    SHVC-3J0N-01
    serial:   SNS-TI-USA
    sha256:   3e487f8ba48c0b5e31744e3281d6bce375089db6075c8eb3d9a929376b817381
    configuration
      rom name=program.rom size=0x280000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Street Hockey '95
    name:     Street Hockey '95
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-AHYE-USA
    sha256:   6756f92fe8e066be4b204cfdc94c1615ba6ece7e78fb5d2c77c81a9d298aa744
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Street Racer
    name:     Street Racer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ASRE-USA
    sha256:   d1f61b6bb4bb6879a4fbd2c82d77390c546ee7f821edddc884fb9cc7463ad79b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Strike Gunner S.T.G.
    name:     Strike Gunner STG
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-SG-USA
    sha256:   05f14e6ed3394d9273e2397769a8acf1a9db646be6066e82269521e8eec53562
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1CA6B revision=01
    superfx revision=4
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x10000
      map id=io address=00-3f,80-bf:3000-34ff
      map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
      map id=rom address=40-5f,c0-df:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff size=0x2000
      map id=ram address=70-71,f0-f1:0000-ffff
  information
    title:    Stunt Race FX
    name:     Stunt Race FX
    region:   NA
    revision: 1.1
    board:    SHVC-1CA6B-01
    serial:   SNS-CQ-USA
    sha256:   c04d80b84514202ff319384ca20641eb0189e975eed5612915bd9c224b2ab30a
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x10000
      superfx revision=4

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Sunset Riders
    name:     Sunset Riders
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6S-USA
    sha256:   e9c406d4f773697b9b671e7ddf2207c9d0ab242d7f23e502cdd453fbb264d392
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Adventure Island
    name:     Super Adventure Island
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-H2-USA
    sha256:   190999122aacc2cff20c5677b3f60ed938d8a36b696d16cc1bf416705efe151e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    Super Adventure Island II
    name:     Super Adventure Island II
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-11
    serial:   SNS-E4-USA
    sha256:   eaf1b83e95d8a04f9a84d4960cf87cc182fc60ef07be35eb8929c4033d6fef67
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Alfred Chicken
    name:     Super Alfred Chicken
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8A-USA
    sha256:   0deb7a91fbe5848f1733ce668daaa49b0dad3d821bacc0791837c1ba15e60d7c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Super Aquatic Games starring the Aquabats
    name:     Super Aquatic Games starring the Aquabats, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-AU-USA
    sha256:   b64f9f95fb244feddb3ff50bf0972af88f5d006e9b09050b930374fef8b37862
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Super Baseball 2020
    name:     Super Baseball 2020
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-01
    serial:   SNS-SA-USA
    sha256:   8808783f789ca6413364a7abea240f6f7291b5906026f360ba8cfdd2791fc179
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x200
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Super Baseball Simulator 1.000
    name:     Super Baseball Simulator 1.000
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-UB-USA
    sha256:   1622371a5a4001fff9690323e89b7a8d449cdc3cae6dcd1249f0c7dc8c651d33
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x200

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Bases Loaded
    name:     Super Bases Loaded
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-SP-USA
    sha256:   a8239355631d303ecebfd43fc14e80f148e4ac9937234e29cc87d6f939b033a0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2B3B revision=01
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram size=0x200
      map id=io address=60-6f,e0-ef:0000-7fff select=0x4000
  information
    title:    Super Bases Loaded 2
    name:     Super Bases Loaded 2
    region:   NA
    revision: 1.0
    board:    SHVC-2B3B-01
    serial:   SNS-3D-USA
    sha256:   327745aceb708d82419c3d2cffdffa78f55df0878bb1786e1b7fc294385f20ba
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram name=dsp1.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Super Bases Loaded 3: License to Steal
    name:     Super Bases Loaded 3 - License to Steal
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-30
    serial:   SNS-AB3E-USA
    sha256:   b21a161fed748920e54cd72c54095416b1d999636e0388d7d147884779c52833
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Batter Up
    name:     Super Batter Up
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-FA-USA
    sha256:   165938810948f3226f7446978fa36ae8bc781616d95b39cd126d5c8afbf6e2ee
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Battleship
    name:     Super Battleship
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-8B-USA
    sha256:   1d6573e3db2efab0c98d1940b4171d569348d27a1cc20e80a83094e6c0e66e35
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Battletank: War in the Gulf
    name:     Super Battletank - War in the Gulf
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-SB-USA
    sha256:   94496e73fc7fdf2f72f16bf2becb0c3935db2ebd97555eac73b63400acbceec6
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Battletank 2
    name:     Super Battletank 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-2X-USA
    sha256:   b68e865b0b5fe6af421a171e94fb1cb0006ae3e412b6361f6f858c44adaa304b
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Black Bass
    name:     Super Black Bass
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-BQ-USA
    sha256:   f0913358cb0870c3dcc7f0c0d2d69704874c31a113150eda668c95024c0d6fd9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x80000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Super Bomberman
    name:     Super Bomberman
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-H6-USA
    sha256:   4efab3f49cbe91ec77b6cba747ddfedfdc0b080c755a8b6ba51234f0676c000f
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Super Bomberman 2
    name:     Super Bomberman 2
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-M4-USA
    sha256:   0a4b4a783a7faf6ada3e1326ecf85de77e8c2a171659b42a78a1fae43f806ca6
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Super Bonk
    name:     Super Bonk
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-11
    serial:   SNS-ANKE-USA
    sha256:   21d4a72461d8680cf75cf3b8eba42e13127815bc17b6249d89a5e39beb3f1406
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Bowling
    name:     Super Bowling
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-BW-USA
    sha256:   946de556b4f877e54e16b5c828db89c038e50349cfd0ddf8ea96b6541f9d70fa
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Buster Brothers
    name:     Super Buster Brothers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-SN-USA
    sha256:   5965bde449ff775c1a0d9fd3cf2fb8c51a86b44ad1942dfb5c14a91f103be030
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Caesers Palace
    name:     Super Caesers Palace
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-C6-USA
    sha256:   d42f8c7969b4c434f9ca04ce0080d897877a5e71c2926d309ef5dae93ba25548
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Castlevania IV
    name:     Super Castlevania IV
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-AD-USA
    sha256:   0ef6f4cce5a2273fa49fe1ce724e0048a8e39c91da6b00dbb693fe1ba909177d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Chase H.Q.
    name:     Super Chase HQ
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-QT-USA
    sha256:   b839253b878821ff00847491d11452e933baaf303f49dd39d22e3a524ea1ff81
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Super Conflict
    name:     Super Conflict
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-06
    serial:   SNS-CN-USA
    sha256:   6cbc4db85cb8420020f9edbfd290c0f435b313a59577aca65314d31f8b816620
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Copa
    name:     Super Copa
    region:   MX
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-75-USA
    sha256:   db7d727de86a1ac886ce3c243bf5941910154eaef3be50209187b9711592830d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Double Dragon
    name:     Super Double Dragon
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-WD-USA
    sha256:   bcced1be76ef920b562a555696bcb4583d1c8cea4d4b057cab6e0e09be8ef8c4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board id:Super Game Boy
    rom name=program.rom size=0x40000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
    icd2 revision=1
      rom name=sgb.boot.rom size=0x100
      map id=io address=00-3f,80-bf:6000-67ff
      map id=io address=00-3f,80-bf:7000-7fff
  information
    title:    Super Game Boy
    name:     Super Game Boy
    region:   NA
    revision: 1.2
    board:    Super Game Boy
    serial:   N/A
    sha256:   4d7fc331a811b8dc630b469262fd6f45e289243cef83101f32038158967d1b28
    configuration
      rom name=program.rom size=0x40000
      rom name=sgb.boot.rom size=0x100

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Ghouls 'n Ghosts
    name:     Super Ghouls & Ghosts
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-CM-USA
    sha256:   7468c271d7240cf4e0d08c16e9969a1b1b1caf5adc0e5adc568d93c92651a057
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Goal! 2
    name:     Super Goal! 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-JV-USA
    sha256:   d4caa6683ee1b6c70c10fd0555ade33dadcc6551d94e791586e64fd683d8f3a8
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Godzilla
    name:     Super Godzilla
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-7G-USA
    sha256:   33dda5838264c93341ef865512e4b86e16fd4a0387eda04e331517bfaecf1c0b
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super High Impact
    name:     Super High Impact
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-HX-USA
    sha256:   d78ff42efdbb0e180777d17b5f7a96e08530ab77f15a99237f60714f7cfe2b74
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super James Pond
    name:     Super James Pond
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-J5-USA
    sha256:   3f8efb19eae68f24feb42c018b7dc7a819bfd8d993ab36899681caa7ee94b06e
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A3B revision=01
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Super Mario All-Stars
    name:     Super Mario All-Stars
    region:   NA
    revision: 1.0
    board:    SHVC-2A3B-01
    serial:   SNS-4M-USA
    sha256:   a9e3e57d591e995e8e0dd228b619b6aed42205eaf55316fa8ff33f236b3a32b3
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=BA3M revision=01
    rom name=program.rom size=0x280000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Super Mario All-Stars + Super Mario World
    name:     Super Mario All-Stars + Super Mario World
    region:   NA
    revision: 1.0
    board:    SHVC-BA3M-01
    serial:   SNS-5M-USA
    sha256:   a8806bfe07cd3c9945d9fd3fcea932ae1cd671cab5cae12bb7a2ae726cbf9175
    configuration
      rom name=program.rom size=0x280000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1K1B revision=01
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram size=0x200
      map id=io address=00-1f,80-9f:6000-7fff select=0x1000
  information
    title:    Super Mario Kart
    name:     Super Mario Kart
    region:   NA
    revision: 1.0
    board:    SHVC-1K1B-01
    serial:   SNS-MK-USA
    sha256:   89ad4ba02a2518ca792cf96b61b36613f86baac92344c9c10d7fab5433bebc16
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram name=dsp1.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1L5B revision=11,20
    sa1
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x8000
      ram size=0x800
      map id=io address=00-3f,80-bf:2200-23ff
      map id=rom address=00-3f,80-bf:8000-ffff
      map id=rom address=c0-ff:0000-ffff
      map id=bwram address=00-3f,80-bf:6000-7fff
      map id=bwram address=40-4f:0000-ffff
      map id=iram address=00-3f,80-bf:3000-37ff
  information
    title:    Super Mario RPG: Legend of the Seven Stars
    name:     Super Mario RPG - Legend of the Seven Stars
    region:   NA
    revision: 1.0
    board:    SHVC-1L5B-11
    serial:   SNS-ARWE-USA
    sha256:   740646f3535bfb365ca44e70d46ab433467b142bd84010393070bd0b141af853
    configuration
      rom name=program.rom size=0x400000
      ram name=save.ram size=0x8000
      ram name=internal.ram size=0x800

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Super Mario World
    name:     Super Mario World
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-06
    serial:   SNS-MW-USA-1
    sha256:   0838e531fe22c077528febe14cb3ff7c492f1f5fa8de354192bdff7137c27f5b
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1CB5B revision=20
    superfx revision=4
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000
      map id=io address=00-3f,80-bf:3000-34ff
      map id=rom address=00-3f:8000-ffff mask=0x8000
      map id=rom address=40-5f:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff size=0x2000
      map id=ram address=70-71:0000-ffff
  information
    title:    Super Mario World 2: Yoshi's Island
    name:     Super Mario World 2 - Yoshi's Island
    region:   NA
    revision: 1.1
    board:    SHVC-1CB5B-20
    serial:   SNS-YI-USA
    sha256:   bd763c1a56365c244be92e6cffefd318780a2a19eda7d5baf1c6d5bd6c1b3e06
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000
      superfx revision=4

release
  cartridge region=NTSC
    board type=BA3M revision=01
    rom name=program.rom size=0x300000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Super Metroid
    name:     Super Metroid
    region:   NA
    revision: 1.0
    board:    SHVC-BA3M-01
    serial:   SNS-RI-USA
    sha256:   12b77c4bc9c1832cee8881244659065ee1d84c70c3d29e6eaf92e6798cc2ca72
    configuration
      rom name=program.rom size=0x300000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Ninja Boy
    name:     Super Ninja Boy
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-CW-USA
    sha256:   40b46bb29785fb431b325f22377faa8b099be4d77aecc1f03000b8a4cb589b63
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Super Nova
    name:     Super Nova
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-01
    serial:   SNS-DH-USA
    sha256:   a8acbbd6f8afbf289a6b837da6cd8bd109a00cd38625c956ab8ff739732d9e4f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Off Road
    name:     Super Off Road
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-OR-USA
    sha256:   24b687f95bb9737d223738f13c00aeaa7d697fa9e2fd50597b81d0cfa2160daa
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Off Road: The Baja
    name:     Super Off Road - The Baja
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-R8-USA
    sha256:   16f9c90d75bd23d0620be00ecf818fcb25c5935d4ee26b1fe17b926f8987aa65
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Pinball: Behind the Mask
    name:     Super Pinball - Behind the Mask
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XP-USA
    sha256:   6bf0e7a7b95613b9d0e5c8cc98eee5d0ac200e88b5d4444ad5cf93d3e8265118
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Pinball: Behind the Mask
    name:     Super Pinball - Behind the Mask
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-30
    serial:   M/SNS-XP-USA-1
    sha256:   65aff1403838c938d7c6170f3044a2b0317cd2ee13ad79353a4cdd2aa82f4e87
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Super Play Action Football
    name:     Super Play Action Football
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-SF-USA
    sha256:   5a4b0c89606f71182fa5552ac476cc3bbda5ddc7d44e33f9184114aaea38020d
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Super Punch-Out!!
    name:     Super Punch-Out!!
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-4Q-USA
    sha256:   a3d803b8c6b0b6ac55085671040b840f993543915c7f802e14fb651beabe9b63
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Putty
    name:     Super Putty
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-YU-USA
    sha256:   3dff3513ae6acb85a02b00b9f5adb4757ab97661993bded4f329127a792cef87
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super R-Type
    name:     Super R-Type
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-SR-USA
    sha256:   05c7f6461209020785fba33007e1830820aa44ada4b1a6f991d936bf2335b15b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super R.B.I Baseball
    name:     Super RBI Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ARLE-USA
    sha256:   607bfa0f74e57298f59ebe237c2e7b59364024d844534f3befb9ad1301bbedf8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Scope 6
    name:     Super Scope 6
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-LR-USA
    sha256:   7a8ffaf8bb549b400ec2f0bda9f3c0dbf5852c38618cdb21cd783c368383e2c7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Slam Dunk
    name:     Super Slam Dunk
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-D9-USA
    sha256:   e987ceb53cc1407b1ba612b13a7b5391db6c41ea14552c165ae62ad42eeaa0ae
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Slap Shot
    name:     Super Slap Shot
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ZX-USA
    sha256:   c39ebf88af138e87ca5d963ed71a8b7defba3e2bc271d72186d00056e8225721
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Smash T.V.
    name:     Super Smash TV
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-TV-USA
    sha256:   8b75ab4bb1b63c45a56e7a4c5a37c0864f14375ab3131edc33489764426ad888
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Soccer
    name:     Super Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-FS-USA
    sha256:   694ad2b34ab808bd8a1aa9dda359594cfd3a5fd9d95d4cf262ccaa0eb57eef67
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Soccer Champ
    name:     Super Soccer Champ
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-HT-USA
    sha256:   5eb9736d66b3ac730ebbfdd19ef2397282b5f1a62dc0048093e041e23b807741
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Solitaire
    name:     Super Solitaire
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-LT-USA
    sha256:   5985cdad45958a5b8c5967ad39efe51569f560bf237a4e9864f21111082a8500
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Star Wars: A New Hope
    name:     Super Star Wars - A New Hope
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-V4-USA
    sha256:   c6bd7239cb2074ff1c471d14535bead6290bba2d7c75b4f03209cb114877b4c8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Star Wars II: The Empire Strikes Back
    name:     Super Star Wars II - The Empire Strikes Back
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-10
    serial:   SNS-E5-USA
    sha256:   1dd01da68cb61def641389bf3fa87b7a29113c9c8dd172cf303c0f7e03ba6e47
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Star Wars II: The Empire Strikes Back
    name:     Super Star Wars II - The Empire Strikes Back
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30-2
    serial:   SNS-E5-USA
    sha256:   46370b3bd6ff9ee04c425eef0f482f521f3b61bd4b058f2f4e9d322253d7b5de
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Star Wars III: Return of the Jedi
    name:     Super Star Wars III - Return of the Jedi
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ARJE-USA
    sha256:   c49be867cdd276703a3ae46a6b17673b9e359af4f9af2329c8eb747120245817
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Star Wars III: Return of the Jedi
    name:     Super Star Wars III - Return of the Jedi
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30-2
    serial:   SNS-ARJE-USA
    sha256:   f7df5cd16ce6624567d1a24e9b9c0b9050ea9b6a9fe5a7973484589637756596
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Super Street Fighter II: The New Challengers
    name:     Super Street Fighter II - The New Challengers
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-XW-USA
    sha256:   d17cb5c73174060fcbd9cba6c705643f19c3b8be24d0f7ee43aee674ff1ea38e
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Strike Eagle
    name:     Super Strike Eagle
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-EG-USA
    sha256:   830c900083cccc6ded74c167c4e257db34df4786ecd6e2f053de454db7d31fe5
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Tennis
    name:     Super Tennis
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-ST-USA
    sha256:   6e45a80ea148654514cb4e8604a0ffcbc726946e70f9e0b9860e36c0f3fa4877
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Troll Islands
    name:     Super Troll Islands
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-5L-USA
    sha256:   02cb7f1ed21ed6bfb9eaa0e91df2adb063a9bf4cbd6feb6cd024d676829e227e
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Turrican
    name:     Super Turrican
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-TU-USA
    sha256:   056ac8160363d577e3ffc2f593801b8bb3d024fe4f3a3331b711dc556204949d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Turrican 2
    name:     Super Turrican 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A2TE-USA
    sha256:   96da3512a1aa05a40f1e5d61c48932b0d55d9f136d6418b848153a9fecab06de
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Valis IV
    name:     Super Valis IV
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-VA-USA
    sha256:   792e615a34ceae5a1b9a4f54c5a5d9b53e39299332fece83e4bceca2c22efb21
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Super Widget
    name:     Super Widget
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-WI-USA
    sha256:   62d3d650e956f23f3b221f83c5c5bd204afd0e4f4d9c47a3251962323de96089
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1K0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram size=0x200
      map id=io address=00-1f,80-9f:6000-7fff select=0x1000
  information
    title:    Suzuka 8 Hours
    name:     Suzuka 8 Hours
    region:   NA
    revision: 1.0
    board:    SHVC-1K0N-01
    serial:   SNS-8H-USA
    sha256:   dfada65f4497f482d5f81964322d9952f678f50e65418d307d4503f9bf9a32d9
    configuration
      rom name=program.rom size=0x100000
      rom name=dsp1.program.rom size=0x1800
      rom name=dsp1.data.rom size=0x800
      ram name=dsp1.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    SWAT Kats: The Radical Squadron
    name:     SWAT Kats - The Radical Squadron
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AK9E-USA
    sha256:   d802715fb4f09d7e499b5b3e577af641598a723dae7cedeaa93943bb53c6edbb
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Syndicate
    name:     Syndicate
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AFYE-USA
    sha256:   c2015d03dd3db08069b2a6ea1ed6b3e1ac1e3a5f804b02295c3cd3717add91ef
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Taz-Mania
    name:     Taz-Mania
    region:   NA
    revision: 1.1
    board:    MAXI-1A0N-30-2
    serial:   SNS-TZ-USA
    sha256:   1711fe9010232b41ec406900e5b4ef528dd2beaa97b27d94ed7eef57d63904a0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Tecmo Secret of the Stars
    name:     Tecmo Secret of the Stars
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   M/SNS-AQ-USA
    sha256:   7da4a3cfa5de4bb4722a6e2a42c26aae322b5e668f1645d8c870fb99e6080600
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Tecmo Super Baseball
    name:     Tecmo Super Baseball
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-11
    serial:   SNS-ATBE-USA
    sha256:   c3cb1d2fd9775aa9d15b7eedd45ad82519b73d47ca166737ed398c748717bcc4
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A3B revision=01
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Tecmo Super Bowl
    name:     Tecmo Super Bowl
    region:   NA
    revision: 1.0
    board:    SHVC-2A3B-01
    serial:   SNS-7T-USA
    sha256:   35dd020cf57fc402417ab6e4a6c49866c5a86bba25218c0aaf7ce85cb134bcf8
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Tecmo Super Bowl II: Special Edition
    name:     Tecmo Super Bowl II - Special Edition
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-11
    serial:   SNS-ASBE-USA
    sha256:   2972057a0087f8239d2523eaa995405f06e5d5ba3a3203b6b50d401379c8ebde
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A5M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x8000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Tecmo Super Bowl III: Final Edition
    name:     Tecmo Super Bowl III - Final Edition
    region:   NA
    revision: 1.0
    board:    SHVC-1A5M-20
    serial:   M/SNS-AW4E-USA
    sha256:   8cfd4c5525f4bd4bba5af7e2323f1e61f27ce97c6d5617cfa685c9276fbf6407
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Tecmo Super NBA Basketball
    name:     Tecmo Super NBA Basketball
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-XM-USA
    sha256:   14bce564f976d1431127259edbcb23c52c79361fed9814d307d627c4650e800e
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Teenage Mutant Ninja Turtles IV: Turtles in Time
    name:     Teenage Mutant Ninja Turtles IV - Turtles in Time
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-TM-USA
    sha256:   5b82cdd6f2da56f43680d6a5021faebe2e06036d30602c1a7917aa414cf8b5f4
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Teenage Mutant Ninja Turtles V: Tournament Fighters
    name:     Teenage Mutant Ninja Turtles V - Tournament Fighters
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-KY-USA
    sha256:   849141370f164d6db3e5709da670726f958ce13ffef69319564db3fb0b12c69d
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Terminator
    name:     Terminator
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-TN-USA
    sha256:   bd7074ef4a05d790646abe145ffd2587fb48044e4b730286d807abe102841177
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Terminator 2: Judgment Day
    name:     Terminator 2 - Judgment Day
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-TP-USA
    sha256:   06db3be569a587d79b51bfc684fd2ebdea977863875aedec88218fbb4169c21b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Terminator 2: The Arcade Game
    name:     Terminator 2 - The Arcade Game
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-XV-USA
    sha256:   365f10f9d9f68cc59e769eeb451c417e1ff7415022a625de9976a3b924c0bd61
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Test Drive II: The Duel
    name:     Test Drive II - The Duel
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-DL-USA
    sha256:   c9563cb57314588aa3db22b76dc8acfba3e73733dd3538edd90af5a15595830e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tetris & Dr. Mario
    name:     Tetris & Dr. Mario
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AFTE-USA-1
    sha256:   3cdebbd8adc4bb6773a7995f542fdac49adefca71cba583255a1c1bf37ac3946
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tetris 2
    name:     Tetris 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-27-USA
    sha256:   accc836c3adabadc810fbe35702c6a64d50a09f4c672d2734fa58b251d7a20aa
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tetris 2
    name:     Tetris 2
    region:   NA
    revision: 1.1
    board:    SHVC-1A0N-30
    serial:   SNS-27-USA-1
    sha256:   70dea29a928c1625def31c862dc74960e39e587e416b45829efc21f13ebd9630
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tetris Attack
    name:     Tetris Attack
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-AYLE-USA-1
    sha256:   579bf1a1988d4af06a69cc1d82a2478ebe51940c5ced7f97e83029a24e6aa778
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Thomas the Tank Engine & Friends
    name:     Thomas the Tank Engine & Friends
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6T-USA
    sha256:   1121df3f05dd9dbb749f6b785eb9b3eb69968004f9d6ceffa6f84757af2f3702
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Thunder Spirits
    name:     Thunder Spirits
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-TH-USA
    sha256:   d50aaa41e153ca356eee16a9deccb1a763ee56ebbe6c80cd28c5ad1db66a5316
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Tick
    name:     Tick, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ATHE-USA
    sha256:   eb958801fd1f08771e5a0933f7701d633262efbfe8d47de21dda18e3b77670de
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Time Cop
    name:     Time Cop
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-ATCE-USA
    sha256:   16fb965130e57f37dda2466f23820f091f8b96758aa7e30ba4fd63cb618e5ddb
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Time Slip
    name:     Time Slip
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XT-USA
    sha256:   8e82f98d2e62bc1e5fcf2386c2b5ca54998398220efcedd67858aaaa92289a42
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Time Trax
    name:     Time Trax
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-X8-USA
    sha256:   fa7e2b40093f0cc7233cc77e95bbbea2144c8183dec10446590396fffd7cda37
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x180000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Timon & Pumbaa's Jungle Games
    name:     Timon & Pumbaa's Jungle Games
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-AJ9E-USA
    sha256:   271a67b32b3bb00ceb0f4e7d81494888d0d821635f0f936d481dfbe671567c08
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Tin Star
    name:     Tin Star
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-9N-USA
    sha256:   0503cd93b4d211a825acd47ff3813668b4ce68890c8be2fbfe5ac2b46882dfcf
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tiny Toon Adventures: Buster Busts Loose!
    name:     Tiny Toon Adventures - Buster Busts Loose!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-TA-USA-1
    sha256:   ba679a11264e9695895a6c17358a41e8459be06166d056811df9c2738fef3d0d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tiny Toon Adventures: Wacky Sports Challenge
    name:     Tiny Toon Adventures - Wacky Sports Challenge
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-5Z-USA
    sha256:   f753de4a38bd83f7d937fc7bf5565a3c351a794c113dead8fdee6d86c85633e8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    TKO Super Championship Boxing
    name:     TKO Super Championship Boxing
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-BX-USA
    sha256:   08d808e9c5851e4301a38a56b350a20ea9e3adbef51546e87e1785d691d0f2d5
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    TNN Bass Tournament of Champions
    name:     TNN Bass Tournament of Champions
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-20
    serial:   M/SNS-ATNE-USA
    sha256:   ced8c0bc2791ffe47cb9eec03db67567945af8c58b5330326722f1cfca41bf51
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tom and Jerry
    name:     Tom and Jerry
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-TJ-USA
    sha256:   4f500da19dbb1557a7bc0ce14437098c1402478d573fb569303b81c011f86fbf
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tommy Moe's Winter Extreme: Skiing and Snowboarding
    name:     Tommy Moe's Winter Extreme - Skiing and Snowboarding
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XS-USA
    sha256:   d99008d2181986dc7f65228696d940b5d31be8471f166d1ab9b1c14f1503bcfb
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tony Meola's Sidekicks Soccer
    name:     Tony Meola's Sidekicks Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-6K-USA
    sha256:   e0fbf731550266d79a0b6ca460afd04c8c312f7023b2c9882c4fc3acc3e7932f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Top Gear
    name:     Top Gear
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-TR-USA
    sha256:   ca9889f17f184b3d99a2eaaa82af73e366f03ed00313fdd369e5e023b208e788
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Top Gear 2
    name:     Top Gear 2
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-2P-USA
    sha256:   76b2702c4be8b668c1017f2817c280283c275eaa41535bf6ffa2b8d2220b68c6
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1B0N revision=02,03
    rom name=program.rom size=0x100000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    necdsp model=uPD7725 frequency=8000000
      rom name=dsp4.program.rom size=0x1800
      rom name=dsp4.data.rom size=0x800
      ram size=0x200
      map id=io address=30-3f,b0-bf:8000-ffff select=0x4000
  information
    title:    Top Gear 3000
    name:     Top Gear 3000
    region:   NA
    revision: 1.0
    board:    SHVC-1B0N-03
    serial:   SNS-A3TE-USA
    sha256:   ede60f0d283b6ea3a45308981d99e7c422a0296e5fadde51c6bafd4dcc789ca2
    configuration
      rom name=program.rom size=0x100000
      rom name=dsp4.program.rom size=0x1800
      rom name=dsp4.data.rom size=0x800
      ram name=dsp4.data.ram size=0x200

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Total Carnage
    name:     Total Carnage
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-XC-USA
    sha256:   9bf884be5627d38f060ad7f3a61ea1fea1474d416e1d037d33014ca9d5205c1d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Toy Story
    name:     Toy Story
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-20
    serial:   SNS-AQHE-USA
    sha256:   345e795000e74f51704774edfc8049473461761a65eb47cab710caa29e09897b
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Toys: Let the Toy Wars Begin!
    name:     Toys - Let the Toy Wars Begin!
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-YT-USA
    sha256:   7a6e5da46b026900fba4584a32ad40d940b9ecf9fccfb11f96a205a914014784
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Troddlers
    name:     Troddlers
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-TX-USA
    sha256:   4070a7702dc506a1ceb6f65b5c330b3a162df6f01735c8f206934fd47b810ed4
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Troy Aikman Football
    name:     Troy Aikman Football
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-11
    serial:   SNS-YQ-USA
    sha256:   46fcca4ce29f472afa8519958d256eec347c2dc2da154c95f263a051c5c02dbb
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    True Golf: Wicked 18
    name:     True Golf - Wicked 18
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-10
    serial:   SNS-W8-USA
    sha256:   dd96a8f4f9c8988301ff710a4c70ebe3bf7914901f3547abe1d5f0dd5c0b921b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    True Golf Classics: Pebble Beach Golf Links
    name:     True Golf Classics - Pebble Beach Golf Links
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-GB-USA
    sha256:   5c7b28bb24bad697156ad444ff23bd15ad6744dbf9899b3cccf2aa36d559d825
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    True Golf Classics: Waialae Country Club
    name:     True Golf Classics - Waialae Country Club
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-11
    serial:   SNS-TG-USA
    sha256:   72088194a65fc057f2910945a33d9f071682a4cecac8996f0bdabcdb5ef39962
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    True Lies
    name:     True Lies
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   SNS-ATLE-USA
    sha256:   47dd8ea2d12a6bb2a9774de1d492259fc4fb9f8ec7976383bbfd922532671f6b
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Tuff E Nuff
    name:     Tuff E Nuff
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-10
    serial:   SNS-TE-USA
    sha256:   8f62d014f513a7dcbca5aa76cbe476c3e4526100f34913af831bc05dab029bd1
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Turn and Burn: No-Fly Zone
    name:     Turn and Burn - No-Fly Zone
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-ZN-USA
    sha256:   e4343c0fadc00ffdc3dc31345068d751eea5d639f826731f08cb81673d508c40
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    The Twisted Tales of Spike McFang
    name:     Twisted Tales of Spike McFang, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-83-USA
    sha256:   259c25d4613f97f5fa7992900fb583625d7fb912c7ae09fa9def2e682834dc9f
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A1M revision=01,11,20
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Ultima: Runes of Virtue II
    name:     Ultima - Runes of Virtue II
    region:   NA
    revision: 1.0
    board:    SHVC-1A1M-11
    serial:   SNS-7U-USA
    sha256:   094555d5720158ee60c9d5ab9a13110192db5ebf0f6cf69abbb59a00bc470345
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Ultima VI: The False Prophet
    name:     Ultima VI - The False Prophet
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-U6-USA
    sha256:   11659bd8dd620d50400d16042aeb2d0ddb00c7183fc1ecb95b1a34f07db0431b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Ultima VII: The Black Gate
    name:     Ultima VII - The Black Gate
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-7I-USA
    sha256:   a31af0e39afb55bbc92a5543b504327fbe7e8cd0a5e08626976bed7b65376737
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ultimate Fighter
    name:     Ultimate Fighter
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-HP-USA
    sha256:   78bf82963cded9162e25035053c8b1a9f760748ff0beacc230d005204992737d
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x400000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Ultimate Mortal Kombat 3
    name:     Ultimate Mortal Kombat 3
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-A3ZE-USA
    sha256:   cb2fdfce61858063bf4c9da4228381c3ec3abe423f4d378cddd174ae4adb261e
    configuration
      rom name=program.rom size=0x400000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Ultraman: Towards the Future
    name:     Ultraman - Towards the Future
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-UM-USA
    sha256:   e9fae4c2e171a1fc4f2bd800abd9e42750aaf7a4db9e40c5b9142e15029500bd
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    U.N. Squadron
    name:     UN Squadron
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-01
    serial:   SNS-E8-USA
    sha256:   0b155a54b6134601fc0791252a63ca73efd522667c3d6fd7a44f5b3c500039d7
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Uncharted Waters
    name:     Uncharted Waters
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-13
    serial:   SNS-QK-USA
    sha256:   794152fc6f55cb15a0b203fa645ac9fa314a293da999d8ec8b3dda080434d175
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1J5M revision=11,20
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x8000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=20-3f,a0-bf:6000-7fff mask=0xe000
  information
    title:    Uncharted Waters: New Horizons
    name:     Uncharted Waters - New Horizons
    region:   NA
    revision: 1.0
    board:    SHVC-1J5M-11
    serial:   SNS-QL-USA
    sha256:   64bc4707f422661a66618088887e2363a5f896ea683c58984fffd96dd21ab5f0
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x8000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x200000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Uniracers
    name:     Uniracers
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-4L-USA
    sha256:   859ec99fdc25dd9b239d9085bf656e4f49c93a32faa5bb248da83efd68ebd478
    configuration
      rom name=program.rom size=0x200000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Untouchables
    name:     Untouchables, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-UC-USA
    sha256:   ecefb4117a6aae117e033c8cc07f0db2797d6be93dd5cdcefc23692a21fae02e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Urban Strike
    name:     Urban Strike
    region:   NA
    revision: 1.0
    board:    MJSC-1A0N-30-2
    serial:   M/SNS-AUSE-USA
    sha256:   dcb33a89fcb8d8ca8f3a467034728ad6375273d8eb51a60418ca51ef745e9b38
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Utopia: The Creation of a Nation
    name:     Utopia - The Creation of a Nation
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-10
    serial:   SNS-UP-USA
    sha256:   2500d6c846c78bcb729f15535bf2b852a120411891cabaaaa6fc407906d0214e
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Vegas Stakes
    name:     Vegas Stakes
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-06
    serial:   SNS-VS-USA
    sha256:   78bf9d79fb2ff3f9d03ecc1176d070e53ddaca2c6b0cda69e74c19a4e50b195b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1CA0N5S revision=01
    superfx revision=4
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x8000
      map id=io address=00-3f,80-bf:3000-34ff
      map id=rom address=00-3f,80-bf:8000-ffff mask=0x8000
      map id=rom address=40-5f,c0-df:0000-ffff
      map id=ram address=00-3f,80-bf:6000-7fff size=0x2000
      map id=ram address=70-71,f0-f1:0000-ffff
  information
    title:    Vortex
    name:     Vortex
    region:   NA
    revision: 1.0
    board:    SHVC-1CA0N5S-01
    serial:   SNS-4V-USA
    sha256:   41b5561de9e4984276e52987ea46c5f4fa8526d8141c70c738875a9eb9fe9d70
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x8000
      superfx revision=4

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Wanderers from Ys III
    name:     Wanderers from Ys III
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-YS-USA
    sha256:   b0e74f0fe8d1e7fe2fe404341fea7c68e28f3a0ab78552d5092d413f2ecec417
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    War 2410
    name:     War 2410
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-A2AE-USA
    sha256:   82bba8ae2bb4dbc74a18af31aaec19c368576e4369dd70b396caa5e8729540bb
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    War 3010: The Revolution
    name:     War 3010 - The Revolution
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AZNE-USA
    sha256:   8b12e5e4553fc921c23739d1aec2ed517535ec239daef800f39f602d8473847f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Wario's Woods
    name:     Wario's Woods
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-21
    serial:   SNS-65-USA
    sha256:   be420715152394e0e4a25ab10931b67f92f910cfcf7793e31dfee1c6334808e5
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Warlock
    name:     Warlock
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AWKE-USA
    sha256:   8579dd352d20589072ed5c026bde7adadd6229d18a022e7cb47cf5602b54015e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Warp Speed
    name:     Warp Speed
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-WP-USA
    sha256:   36ec2f409f08a08f8327570eadcd8960b6a47bf5797441c2df05fcc50c5b762b
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Wayne Gretzky and the NHLPA All-Stars
    name:     Wayne Gretzky and the NHLPA All-Stars
    region:   NA
    revision: 1.0
    board:    SHVC-2A3M-20
    serial:   M/SNS-AWZE-USA
    sha256:   dd73690dd3165a16580e191c92a497102758f312c759353f685e371755c663a8
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wayne's World
    name:     Wayne's World
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-WW-USA
    sha256:   56ba3d585bf6b701e342d86a0bd164ab0a97dfbd5df46b3a964506842633459c
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=2A0N revision=01,10,11,20
    rom name=program.rom size=0x180000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    WCW Super Brawl Wrestling
    name:     WCW Super Brawl Wrestling
    region:   NA
    revision: 1.0
    board:    SHVC-2A0N-11
    serial:   SNS-AWRE-USA
    sha256:   215ab7e576b31462e3284d035006dc638cae06bbfb1af2c5617403b15ab2b25a
    configuration
      rom name=program.rom size=0x180000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    WeaponLord
    name:     WeaponLord
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AWDE-USA
    sha256:   0b1ba31ae95b61d7d9a0f5509b5836fff84f60915802e3b3ba1170a5c50a4b71
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    We're Back!: A Dinosaur's Story
    name:     We're Back! - A Dinosaur's Story
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-6D-USA
    sha256:   e931c3c08f20f78e3a43ad92d16eb472be619abaa17c2d8e2b0fcd5d05dbd74d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wheel of Fortune
    name:     Wheel of Fortune
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-WL-USA
    sha256:   12abf1ba063c120c1a98495a1c85e67a0007aff771ef92adcb94b4a0a2fd5adb
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wheel of Fortune: Deluxe Edition
    name:     Wheel of Fortune - Deluxe Edition
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30
    serial:   SNS-XF-USA
    sha256:   c7af9e7f3243ba1b5dd81f32f710e60c2ae1d443ecaf7140bf42d71a1b69d8a2
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Where in the World is Carmen Sandiego?
    name:     Where in the World is Carmen Sandiego
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-WX-USA
    sha256:   1bd3af0125743bf9bbbac6e7dc215fe32c4ff7043a2ee034d56b627e3f6875f0
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Where in Time is Carmen Sandiego?
    name:     Where in Time is Carmen Sandiego
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-WN-USA
    sha256:   e3177f066bf004fd99aef31f2692ba0b412dd280d613dc3f4cf334d97f4c9af8
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Whizz
    name:     Whizz
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-AZWE-USA
    sha256:   0a52dc1e7820f5541f53ce0e1e96144fe079af0efe3dae5c2c89d86365feb8b1
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wild Guns
    name:     Wild Guns
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-30
    serial:   M/SNS-4W-USA
    sha256:   c8f159e2625ac8078535c06857ea28475706da45df494de8e46f50888272cf71
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wild Snake
    name:     Wild Snake
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AWSE-USA
    sha256:   39bb99eddb224de5f0789f807ffef06b9efa2efb7962dced31fb272f986699cd
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Williams Arcade's Greatest Hits
    name:     Williams Arcade's Greatest Hits
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-AW8E-USA
    sha256:   60d01a3f499463156546ecdee18ee3e946b95302ee0b1569decb97f52372d2eb
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wing Commander
    name:     Wing Commander
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-WC-USA
    sha256:   2167fc7c5447b2287668d2f3e4ef1a285361b2292ecc8a4cbd9f966a460ad7a2
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wing Commander: The Secret Missions
    name:     Wing Commander - The Secret Missions
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-2W-USA
    sha256:   132ca0b6a4888edf7de785d48f4417fac28522646e6c7514f80c5e9ff1438d5f
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wings 2: Aces High
    name:     Wings 2 - Aces High
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-02
    serial:   SNS-WG-USA
    sha256:   c3bcd5c716f96e6359ebcfd85c3e9b07b46c5124bf4010d89ceef5b6f2f868f6
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Winter Olympic Games: Lillehammer '94
    name:     Winter Olympic Games - Lillehammer '94
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-W4-USA
    sha256:   0e834647669783c2b79cc1120c057c870c541079a7abd1eee3f787d59dc3c3eb
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    The Wizard of Oz
    name:     Wizard of Oz, The
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-W6-USA
    sha256:   025dd3047c474d879e69b91a3918add9cdabedf4182e1c1e10e5f0c13a124bf9
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A3M revision=10,20,21,30
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-7fff
  information
    title:    Wizardry V: Heart of the Maelstrom
    name:     Wizardry V - Heart of the Maelstrom
    region:   NA
    revision: 1.0
    board:    SHVC-1A3M-20
    serial:   SNS-W5-USA
    sha256:   bc68f40075f9983f5027fe264c3037d1a831e8e76a6c9adb60d10226f6ef005b
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wolf Child
    name:     Wolf Child
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-WH-USA
    sha256:   e0165bafeb8d65be08a5a4079f8651104471f450c60794b761b1255853ca2d98
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x100000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Wolfenstein 3D
    name:     Wolfenstein 3D
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-10
    serial:   SNS-6W-USA
    sha256:   9c2b458e8fda5cb437a4c6d28fb430e45c4cfef98420c40546b8e08563a4fc7d
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wolverine: Adamantium Rage
    name:     Wolverine - Adamantium Rage
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AWXE-USA
    sha256:   44428a3d1c796fbd41da7620e321c45f11cd80a0e5f4ab8c48177106cb960d77
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Wordtris
    name:     Wordtris
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-WT-USA
    sha256:   82e2b636e66c4edbae27a5be91a61194ef2881ec93f40d1de93a6153617b12f2
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=2J3M revision=01,11,20
    rom name=program.rom size=0x180000
    ram name=save.ram size=0x2000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
    map id=ram address=10-1f,30-3f:6000-7fff mask=0xe000
    map id=ram address=90-9f,b0-bf:6000-7fff mask=0xe000
  information
    title:    World Cup USA '94
    name:     World Cup USA '94
    region:   NA
    revision: 1.0
    board:    SHVC-2J3M-11
    serial:   SNS-U4-USA
    sha256:   86e9d76a8834732e511253d8092727bdbfb409b3d0ff1c06b6b6e481c9a039ef
    configuration
      rom name=program.rom size=0x180000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=2J0N revision=01,10,11
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    World Heroes
    name:     World Heroes
    region:   NA
    revision: 1.0
    board:    SHVC-2J0N-10
    serial:   SNS-WZ-USA
    sha256:   ea76cfdbb2a555a7b6eff8b466a879f9a9189639416e8c2fb45bf074e695105f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=BA0N revision=01,10
    rom name=program.rom size=0x300000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    World Heroes 2
    name:     World Heroes 2
    region:   NA
    revision: 1.0
    board:    SHVC-BA0N-01
    serial:   SNS-JI-USA
    sha256:   159d5341d13d6801324e8271f7191c0223617c9d30984676319b2df7937c78c0
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A1B revision=04,05,06
    rom name=program.rom size=0x80000
    ram name=save.ram size=0x800
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    World League Soccer
    name:     World League Soccer
    region:   NA
    revision: 1.0
    board:    SHVC-1A1B-05
    serial:   SNS-WS-USA
    sha256:   d4d9f1b41dad7e7a126a9adbe8d86c4b339e120c866156796de1cb0c9a214189
    configuration
      rom name=program.rom size=0x80000
      ram name=save.ram size=0x800

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    World Soccer '94: Road to Glory
    name:     World Soccer '94 - Road to Glory
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-WO-USA
    sha256:   2143bbd87ea1c5cfe5eaf46ae39e3ebb11a2e929d05cbb929904037f4d72acfe
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=BJ0N revision=01,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    WWF Raw
    name:     WWF Raw
    region:   NA
    revision: 1.0
    board:    SHVC-BJ0N-01
    serial:   SNS-AWFE-USA
    sha256:   0af7b0d3022acd24a1fb15865a076519f7f56e7a4b33f12b6d851b3a91e5388c
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    WWF Royal Rumble
    name:     WWF Royal Rumble
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-WU-USA
    sha256:   51c53e36ed0b959b0695fc6ef036fa7302d1c995eca35c28261d6f3cb77df0ca
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    WWF Super Wrestlemania
    name:     WWF Super Wrestlemania
    region:   NA
    revision: 1.0
    board:    MAXI-1A0N-30-2
    serial:   SNS-WF-USA
    sha256:   0b9abf2fc25a5f07c71f9d8efbb0d0e616c1494060138fbb63f7398e9c26198e
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x300000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    WWF Wrestlemania: The Arcade Game
    name:     WWF Wrestlemania - The Arcade Game
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   SNS-AWVE-USA
    sha256:   67faa6ed3406a5ab0d7224b811c0960bb36560040ee959bb3304c9293ceaa093
    configuration
      rom name=program.rom size=0x300000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    X-Kaliber 2097
    name:     X-Kaliber 2097
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-X7-USA
    sha256:   dc3792e9fe7ef7aaea4ac675a48ad06129dd3ebdd4b96a513bc8241549cbd579
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x200000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    X-Men: Mutant Apocalypse
    name:     X-Men - Mutant Apocalypse
    region:   NA
    revision: 1.0
    board:    MAXI-1J0N-20-1
    serial:   SNS-AXME-USA
    sha256:   65fe17fd6b297f52df6ce9812ecb02c3bb1bfda3ebc05a19c4a8decbf9a446ae
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    X-Zone
    name:     X-Zone
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-10
    serial:   SNS-XZ-USA
    sha256:   93272180090e8418582f69b79c5cee6b28638b9a93192cc4bcd96291a4fca02d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=1A3B revision=11,12,13
    rom name=program.rom size=0x100000
    ram name=save.ram size=0x2000
    map id=rom address=00-1f,80-9f:8000-ffff mask=0x8000
    map id=ram address=70-7d,f0-ff:0000-ffff
  information
    title:    Xardion
    name:     Xardion
    region:   NA
    revision: 1.0
    board:    SHVC-1A3B-12
    serial:   SNS-XA-USA
    sha256:   71b69490c78d0bbaf47da25217c5dae295190311aa5df75653c3fac0a1b45358
    configuration
      rom name=program.rom size=0x100000
      ram name=save.ram size=0x2000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x80000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Yoshi's Cookie
    name:     Yoshi's Cookie
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-YC-USA
    sha256:   90ad69a489194aca7ef7b7fd1d30e0105da4934a81ac8b0333ea20f9248df92d
    configuration
      rom name=program.rom size=0x80000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Yoshi's Safari
    name:     Yoshi's Safari
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-RH-USA
    sha256:   12fba2aff04c8e39968e828629ebd16caa314bca397a9418d35fdaffe8188e20
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Young Merlin
    name:     Young Merlin
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-Y6-USA
    sha256:   fbe8926fc0149d3e8e2aec20f15640ea6814f4f4b01c3960f3c477f5f17e890f
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x200000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Zero the Kamikaze Squirrel
    name:     Zero the Kamikaze Squirrel
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-AZKE-USA
    sha256:   7d414b7f5941f1eddc35259a22accbbbd7b47c517dfcf8bad86c4dcfa9e50b1e
    configuration
      rom name=program.rom size=0x200000

release
  cartridge region=NTSC
    board type=YA0N revision=01
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Zombies Ate My Neighbors
    name:     Zombies Ate My Neighbors
    region:   NA
    revision: 1.0
    board:    SHVC-YA0N-01
    serial:   SNS-ZA-USA
    sha256:   b27e2e957fa760f4f483e2af30e03062034a6c0066984f2e284cc2cb430b2059
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1A0N revision=01,02,10,20,30
    rom name=program.rom size=0x100000
    map id=rom address=00-7d,80-ff:8000-ffff mask=0x8000
    map id=rom address=40-7d,c0-ff:0000-7fff mask=0x8000
  information
    title:    Zool: Ninja of the "Nth" Dimension
    name:     Zool - Ninja of the 'Nth' Dimension
    region:   NA
    revision: 1.0
    board:    SHVC-1A0N-20
    serial:   SNS-Z8-USA
    sha256:   25414de02c6805ca62574cfb39c23bf292b3d8c4ff33eb8f212ccdbcd61c5ae3
    configuration
      rom name=program.rom size=0x100000

release
  cartridge region=NTSC
    board type=1J0N revision=01,10,20
    rom name=program.rom size=0x80000
    map id=rom address=00-3f,80-bf:8000-ffff
    map id=rom address=40-7d,c0-ff:0000-ffff
  information
    title:    Zoop
    name:     Zoop
    region:   NA
    revision: 1.0
    board:    SHVC-1J0N-20
    serial:   M/SNS-AZPE-USA
    sha256:   a3f1abf2740ff64042d82f7a940fb4269ece57d7c6967571549d8b60b20f305a
    configuration
      rom name=program.rom size=0x80000

)";
