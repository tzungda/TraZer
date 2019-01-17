#ifndef TZ_TRACER_IMAGE_TEXTURE
#define TZ_TRACER_IMAGE_TEXTURE

#include "../include/tzImage.h"
#include "../include/tzIMapping.h"
#include "../include/tzITexture.h"

class tzImageTexture: public tzITexture {
	public:
	
		tzImageTexture(void);
			
		tzImageTexture(tzImage* _image_ptr);
				
		tzImageTexture(const tzImageTexture& it);

		tzImageTexture&
		operator= (const tzImageTexture& rhs);

		virtual tzImageTexture*
		clone(void) const;				

		virtual
		~tzImageTexture(void) ;
		
		virtual tzColor																				
		getColor(const tzShadeRec& sr) const;
						
		void
		set_image(tzImage* _image_ptr);
		
		void
		set_mapping(tzIMapping* map_ptr);	
		
	private:
	
		int 		hres;			// horizontal resolution of the image
		int			vres;			// vertical resolution of the image
		tzImage*		image_ptr;		// the image
		tzIMapping*	mapping_ptr;	// mapping technique used, if any
};


// ---------------------------------------------------------------- set_image

inline void
tzImageTexture::set_image(tzImage* _image_ptr) {
	image_ptr = _image_ptr;
	hres = image_ptr->getHeight();
	vres = image_ptr->getWidth();
}


// ---------------------------------------------------------------- set_mapping

inline void
tzImageTexture::set_mapping(tzIMapping* map_ptr) {
	mapping_ptr = map_ptr;
}

#endif		