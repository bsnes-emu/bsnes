auto Program::filterSelect(uint& width, uint& height, uint scale) -> Filter::Render {
  Filter::Size size = &Filter::None::size;
  Filter::Render render = &Filter::None::render;

  //HD mode 7 is incompatible with software filters
  if(scale != 1) {
    size(width, height);
    return render;
  }

  if(presentation.filterScanlinesLight.checked() && width <= 512 && height <= 240) {
    size = &Filter::ScanlinesLight::size;
    render = &Filter::ScanlinesLight::render;
  }

  if(presentation.filterScanlinesDark.checked() && width <= 512 && height <= 240) {
    size = &Filter::ScanlinesDark::size;
    render = &Filter::ScanlinesDark::render;
  }

  if(presentation.filterScanlinesBlack.checked() && width <= 512 && height <= 240) {
    size = &Filter::ScanlinesBlack::size;
    render = &Filter::ScanlinesBlack::render;
  }

  if(presentation.filterPixellate2x.checked() && width <= 512 && height <= 480) {
    size = &Filter::Pixellate2x::size;
    render = &Filter::Pixellate2x::render;
  }

  if(presentation.filterScale2x.checked() && width <= 256 && height <= 240) {
    size = &Filter::Scale2x::size;
    render = &Filter::Scale2x::render;
  }

  if(presentation.filter2xSaI.checked() && width <= 256 && height <= 240) {
    size = &Filter::_2xSaI::size;
    render = &Filter::_2xSaI::render;
  }

  if(presentation.filterSuper2xSaI.checked() && width <= 256 && height <= 240) {
    size = &Filter::Super2xSaI::size;
    render = &Filter::Super2xSaI::render;
  }

  if(presentation.filterSuperEagle.checked() && width <= 256 && height <= 240) {
    size = &Filter::SuperEagle::size;
    render = &Filter::SuperEagle::render;
  }

  if(presentation.filterLQ2x.checked() && width <= 256 && height <= 240) {
    size = &Filter::LQ2x::size;
    render = &Filter::LQ2x::render;
  }

  if(presentation.filterHQ2x.checked() && width <= 256 && height <= 240) {
    size = &Filter::HQ2x::size;
    render = &Filter::HQ2x::render;
  }

  if(presentation.filterNTSC_RF.checked() && width <= 512 && height <= 480) {
    size = &Filter::NTSC_RF::size;
    render = &Filter::NTSC_RF::render;
  }

  if(presentation.filterNTSC_Composite.checked() && width <= 512 && height <= 480) {
    size = &Filter::NTSC_Composite::size;
    render = &Filter::NTSC_Composite::render;
  }

  if(presentation.filterNTSC_SVideo.checked() && width <= 512 && height <= 480) {
    size = &Filter::NTSC_SVideo::size;
    render = &Filter::NTSC_SVideo::render;
  }

  if(presentation.filterNTSC_RGB.checked() && width <= 512 && height <= 480) {
    size = &Filter::NTSC_RGB::size;
    render = &Filter::NTSC_RGB::render;
  }

  size(width, height);
  return render;
}
