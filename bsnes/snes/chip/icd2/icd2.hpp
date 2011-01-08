class ICD2 : public GameBoy::Interface, public Coprocessor, public MMIO, public Memory {
public:
  static void Enter();
  void enter();

  void init();
  void enable();
  void power();
  void reset();

private:
  #include "interface/interface.hpp"
  #include "mmio/mmio.hpp"
};

extern ICD2 icd2;
