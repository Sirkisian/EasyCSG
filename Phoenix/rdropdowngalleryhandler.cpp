#include "rdropdowngalleryhandler.hpp"

RDropDownGalleryHandler::RDropDownGalleryHandler(_IN_(BOOL & enabled), _IN_(std::vector<Property> & galleryContent)):
RGalleryControlHandler(NULL, enabled, galleryContent)
{
}

RDropDownGalleryHandler::~RDropDownGalleryHandler()
{
}