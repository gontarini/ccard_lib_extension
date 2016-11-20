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
        uint32_t* _lengthPointer;

        /**
        *   Variable used only toString() method which indicates output place for native method
        */
        char* _bitmapStorage;
    public:
        /**
        *  c++ constructor
        */
       HyperLogLogPlusPlus() = default;

        /**
        *  c++ destructor, TODO add destroying ctx
        */
        virtual ~HyperLogLogPlusPlus(){
    
        }

        virtual void __destruct()
        {
            std::cout << "__destruct" << std::endl;
        }
    
         /**
         *  php "constructor"
         *  @param  either bitmap_length or bitma_length with bitmap object
         *  Creating native object - hllp_cnt_init(), creating context for library
         */
        void __construct(Php::Parameters &params)
        {
            // if (params.size() == 1){
            //    std::cout<<"Invoking hllp_cnt_init() without serialized object from extending library"<<std::endl;
            //    Php::Value self(this);
            //    self["_length"] = params[0];
            //    ctx = hllp_cnt_init(NULL, _length);
            //    std::cout<< ctx <<" " << getLength() << std::endl;
            // }else if (params.size() == 2){
            //     std::cout<<"Invoking hllp_cnt_init() without serialized object from extending library"<<std::endl;
            //     ctx = hllp_cnt_init(params[0], params[0].length());

            // }
            // else{
            //     std::cout<<"Invalid amount of argument passed"<<std::endl;
            //     std::cout<<"Accepted amounts are either 1 or 2!"<<std::endl;
            // }
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

        void initSerialized(){
            std::cout<<"Invoking hllp_cnt_init() with serialized object from extending library"<<std::endl;
            std::cout<<"_bitmap " <<_bitmap << " length" << _length <<std::endl;

            ctx = hllp_cnt_raw_init(_bitmap, _length);
            std::cout<<"Created context: " <<ctx << std::endl;
        }

        void init(){
            std::cout<<"Invoking hllp_cnt_init() without serialized object from extending library"<<std::endl;
            ctx = hllp_cnt_init(NULL, _length);
            std::cout<<"Created context: " <<ctx << std::endl;
        }
        
        /**
        * Adding element to context instance
        * Method connected to native hllp_cnt_offer method
        */
        Php::Value offer(Php::Parameters &params){
            std::cout<< "Adding element " << params[0] << " of size " <<sizeof(params[0]) <<" to context" << std::endl;
            int result = hllp_cnt_offer(ctx, params[0], sizeof(params[0]));
            return result;
        }

        /**
        * Computing numerical amount and returning result
        * Native C method is hllp_cnt_card
        */
        Php::Value count(){
            // std::cout<<hllp_cnt_card(ctx)<< std::endl;
            return hllp_cnt_card(ctx);
        }

        /**
        * Get the serialized bitmap or bitmap length from context.
        */
        Php::Value toString(){
            int res = hllp_cnt_get_raw_bytes(ctx, &_bitmapStorage, _lengthPointer);
            if (res!= -1){
                std::cout<<"toString proceeded correctly"<< std::endl;
                return _bitmapStorage;
            }
            else{
                std::cout<<"Error in toString method. Result code "<< res << endl;
                return -1;
            }

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
        * Context getter
        */
        hllp_cnt_ctx_t* getContext(){
            return ctx;
        }

        uint32_t* getLenPointer(){
            return _lengthPointer;
        }

        /**
        * Merging class instances but in serialized mode
        * @params serialized object, string
        * Associated method in native C lib is hllp_merge_bytes
        */
        void mergeRaw(Php::Parameters &params){
            Php::Value value = params[1];
            HyperLogLogPlusPlus *t = (HyperLogLogPlusPlus*) value.implementation();
            uint32_t *size = t->getLenPointer();
            uint32_t s = 1027;
            int result = hllp_cnt_merge_raw_bytes(ctx, params[0], s);
            std::cout<<"mergeRaw result = "<<result << std::endl;
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

        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::init>("init",{});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::initSerialized>("initSerialized",{});
        // hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::__construct>("__construct",{
        //     Php::ByRef("int", Php::Type::Numeric)
        // });
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