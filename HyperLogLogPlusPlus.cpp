#include "HyperLogLogPlusPlus.h"

extern "C"{
    #include "ccard-lib/include/hyperloglogplus_counting.h"
}

using namespace std;

class HyperLogLogPlusPlus : public Php::Base{
    private:
        /**
        * Context
        */
        hllp_cnt_ctx_t *_ctx;

        /**
        * Pointer to bitmap length
        */
        int _length;
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
        virtual void __destruct(){
            hllp_cnt_fini(_ctx);
        }
    
        /**
        * Disable copy constructor
        */
        HyperLogLogPlusPlus(const HyperLogLogPlusPlus &hyperLogLogPlusPlus) = delete;

         /**
         *  php "constructor"
         *  @param  either bitmap_length or bitma_length with bitmap object
         *  Creating native object - hllp_cnt_init(), creating context for library
         */
        void __construct( Php::Parameters &params ){
            if( params.size() == 2 ) {
                const char* _bitmap = params[1];
                _length = params[0];
                _ctx = hllp_cnt_init(_bitmap, _length);
                if (_ctx == NULL){
                    throw Php::Exception("Initialization error. Counting algorithm, hash function or length not match.");
                }
            } else {
                _length = params[0];
                _ctx = hllp_cnt_init(NULL, _length);
                if (_ctx == NULL){
                    throw Php::Exception("Initialization error.");
                }
            }
        }
        
        /**
        * Adding element to context instance
        * Method connected to native hllp_cnt_offer method
        */
        Php::Value offer(Php::Parameters &params){
            int result = hllp_cnt_offer(_ctx, params[0], _length);
            if (result== -1){
                return false;
            }
            return true;
        }


        /**
        * Get the serialized bitmap or bitmap length from context.
        */
        Php::Value toString(){
            uint32_t _length32 = (int)pow(2,_length)+3 ;
            char* _bitmapStorage = new char[_length32]();
            int res = hllp_cnt_get_bytes(_ctx,_bitmapStorage, &_length32);

            if (res!= -1){
                return Php::Value(_bitmapStorage,_length32);
            }
            else{
                return false;  
            }
        }

        /**
        * Computing numerical amount and returning result
        * Native C method is hllp_cnt_card
        */
        Php::Value count(){
            return hllp_cnt_card(_ctx);
        }

        /**
        * Merging this class instance with the instance given as a parameter
        * @params instance of class itself
        * Associated method in native C lib is hllp_cnt_merge
        */
        Php::Value merge(Php::Parameters &params){
            Php::Value value = params[0];
            HyperLogLogPlusPlus *t = (HyperLogLogPlusPlus*) value.implementation();

            hllp_cnt_ctx_t *tmb = t->_ctx;

            int result = hllp_cnt_merge(_ctx, tmb, NULL);
            if(result) {
                return false;
            }
            else{
                return true;
            }

        }

        /**
        * Merging class instances but in serialized mode
        * @params[in] serialized object, string and length of the bitmap
        * @params[in] length of the bitmap
        * Associated method in native C lib is hllp_merge_bytes
        */
        Php::Value mergeRaw(Php::Parameters &params){
            int _lengthMerge = params[0].size();
            int result = hllp_cnt_merge_raw_bytes(_ctx, params[0], _lengthMerge, NULL);
            if(result) {
                return false;
            }
            else{
                return true;
            }
        }

        /**
        * Getter for length of the bitmap
        */
        Php::Value getLength() const{
            return _length;
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
        hyperLogLogPlusPlus.property("bits", &HyperLogLogPlusPlus::getLength);

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::__construct>("__construct",{
            Php::ByVal("size", Php::Type::Numeric),
            Php::ByVal("data", Php::Type::String, false)
        });

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::toString>("toString",{});  

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::count>("count", {});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::offer>("offer", {
            Php::ByVal("object", Php::Type::String)
        });

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::merge>("merge", {
            Php::ByVal("hyperObject", "HyperLogLogPlusPlus")
        });
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::mergeRaw>("mergeRaw", {
            Php::ByVal("bitmap", Php::Type::String),
        });

        // add the class to the extension
        hyperExtension.add(std::move(hyperLogLogPlusPlus));


        // return the extension
        return hyperExtension;
    }
}