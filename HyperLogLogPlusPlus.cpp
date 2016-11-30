#include "HyperLogLogPlusPlus.h"

extern "C"{
    #include "ccard-lib/include/hyperloglogplus_counting.h"
}

using namespace std;

class HyperLogLogPlusPlus : public Php::Base{
    private:
        /**
        *   Variable storing buffer of serialized object
        */
        const char* _bitmap;

        /**
        * Context
        */
        hllp_cnt_ctx_t *ctx;

        /**
        * Pointer to bitmap length
        */
        int _length;

        /**
        *   Pointer for length storage variable
        */
        uint32_t _length32;

        /**
        *   Variable used only toString() method which indicates output place for native method
        */
        char* _bitmapStorage ;

        /**
        *   Length of merging bitmap
        */
        int _lengthMerge;
    public:
        /**
        *  c++ constructor
        */
        HyperLogLogPlusPlus() = default;

        /**
        *  c++ destructor
        */
        virtual ~HyperLogLogPlusPlus(){}

        /**
        * Php magic destructor
        */
        virtual void __destruct()
        {
            std::cout << "__destruct" << std::endl;
        }
    
         /**
         *  php "constructor"
         *  @param  either bitmap_length or bitma_length with bitmap object
         *  Creating native object - hllp_cnt_init(), creating context for library
         */
        void __construct( Php::Parameters &params ){
            std::cout << "construct size " << params[0] << "\n";
            if( params.size() == 2 ) {
                std::cout << "construct data " << params[1] << "\n";
                _bitmap = params[1];
                _length = params[0];
                initSerialized();
            } else {
                _length = params[0];
                init();
            }
        }


        /**
        *   Initilize hllp_cnt_t context without serialized object
        */
        void init(){
            ctx = hllp_cnt_init(NULL, _length);
            std::cout<<"Created context: " <<ctx << std::endl;
            std::cout<<std::endl;
        }

        /**
        *   Initilize hllp_cnt_t context with serialized object
        */
        void initSerialized(){
            ctx = hllp_cnt_raw_init(_bitmap, _length);
            std::cout<<"Created context: " <<ctx << std::endl;
            std::cout<<std::endl;
        }
        
        /**
        * Adding element to context instance
        * Method connected to native hllp_cnt_offer method
        */
        Php::Value offer(Php::Parameters &params){
            std::cout<< "Adding element " << params[0]  <<" to context" << std::endl;
            int result = hllp_cnt_offer(ctx, params[0], sizeof(params[0]));//params[0], sizeof(params[0]));
            return result;
        }


        /**
        * Get the serialized bitmap or bitmap length from context.
        */
        Php::Value toString(){
            _length32 = (int)pow(2,_length)+3 ;
            _bitmapStorage = new char[_length32]();
            int res = hllp_cnt_get_bytes(ctx,_bitmapStorage, &_length32);

            if (res!= -1){
                // std::string result = new string(reinterpret_cast<char*>(*_bitmapStorage),_length32);

                return Php::Value(_bitmapStorage,_length32);
                // return _bitmapStorage;
            }
            else{
                std::cout<<"Error in toString method. Result code "<< res << endl;
                return -1;  
            }
        }


        /**
        * Computing numerical amount and returning result
        * Native C method is hllp_cnt_card
        */
        Php::Value count(){
            return hllp_cnt_card(ctx);
        }

        /**
        * Merging this class instance with the instance given as a parameter
        * @params instance of class itself
        * Associated method in native C lib is hllp_cnt_merge
        */
        void merge(Php::Parameters &params){
            Php::Value value = params[0];
            HyperLogLogPlusPlus *t = (HyperLogLogPlusPlus*) value.implementation();

            hllp_cnt_ctx_t *tmb = t->getContext();

            int result = hllp_cnt_merge(ctx, tmb, NULL);
            std::cout<<result << " <--- result of Merging (0 - positive, -1 - negative)"<< std::endl;
        }



        /**
        * Merging class instances but in serialized mode
        * @params[in] serialized object, string and length of the bitmap
        * @params[in] length of the bitmap
        * Associated method in native C lib is hllp_merge_bytes
        */
        void mergeRaw(Php::Parameters &params){
            _lengthMerge = params[1];
            std::cout<< "Bitmap: "<< params[0] << " length "<<params[1]<<" to be merged" << std::endl;

            int result = hllp_cnt_merge_raw_bytes(ctx, params[0], _lengthMerge, NULL);
            if(result) {
                printf("Result %d ,failed to merge bitmaps: %s",result,hllp_cnt_errstr(hllp_cnt_errnum(ctx)));
            }
            else{
                printf("Succesfully merged!\n\n");
            }
        }

        /**
        * Setter of bitmap length to be merged
        */
        void setLengthMerge(const Php::Value &lengthMerge){
            _lengthMerge = lengthMerge;
        }

        Php::Value getLengthMerge()const{
            return _lengthMerge;
        }

        /**
        * Context getter
        */
        hllp_cnt_ctx_t* getContext(){
            return ctx;
        }

        /**
        *   Storage for bitmap length getter
        */
        uint32_t getLenPointer(){
            return _length32;
        }

                /**
        *  Setter for length of the bitmap
        */
        void setLength(const Php::Value &length){
            _length = (int)length;
            std::cout<<"Bitmap length loaded to property ="<<_length<<std::endl;
        }

        /**
        * Getter for length of the bitmap
        */
        Php::Value getLength() const{
            return _length;
        }

        void setBitmap(const Php::Value &bitmap){
            _bitmap = bitmap;
            std::cout<<"Bitmap loaded to property ="<<_bitmap<<std::endl;
        }

        Php::Value getBitmap() const{
            return _bitmap;
        }
};


/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module()
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension hyperExtension("HyperLogLogPlusPlus", "1.0");
 
        Php::Class<HyperLogLogPlusPlus> hyperLogLogPlusPlus("HyperLogLogPlusPlus");
        hyperLogLogPlusPlus.property("_length", &HyperLogLogPlusPlus::getLength, &HyperLogLogPlusPlus::setLength);
        hyperLogLogPlusPlus.property("_bitmap", &HyperLogLogPlusPlus::getBitmap, &HyperLogLogPlusPlus::setBitmap);
        hyperLogLogPlusPlus.property("ctx", &HyperLogLogPlusPlus::getContext);
        hyperLogLogPlusPlus.property("_lengthPointer", &HyperLogLogPlusPlus::getLenPointer);
        hyperLogLogPlusPlus.property("_lengthMerge", &HyperLogLogPlusPlus::getLengthMerge, &HyperLogLogPlusPlus::setLengthMerge);

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::init>("init",{});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::initSerialized>("initSerialized",{});

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::__construct>("__construct",{
            Php::ByVal("size", Php::Type::Numeric),
            Php::ByVal("data", Php::Type::String, false)
        });

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::toString>("toString",{});  

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::count>("count", {});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::offer>("offer", {
            Php::ByVal("object", Php::Type::String)
        });

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::merge>("merge", {});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::mergeRaw>("mergeRaw", {
            Php::ByVal("bitmap", Php::Type::String),
            Php::ByVal("len", Php::Type::Numeric)
        });

        // add the class to the extension
        hyperExtension.add(std::move(hyperLogLogPlusPlus));


        // return the extension
        return hyperExtension;
    }
}