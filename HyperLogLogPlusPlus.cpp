#include "HyperLogLogPlusPlus.h"

extern "C"{
    #include "ccard-lib/include/hyperloglogplus_counting.h"
}

using namespace std;

class HyperLogLogPlusPlus : public Php::Base{
    private:
        /**
        *   Estimating object
        */
        int _payload;

        /**
        * Serialized bitmap 
        */
        string serialized;

        int value;

        /**
        *   Payload integer
        */
        Php::Value _payload_int;

        /**
        *   Payload string
        */
        // Php::Value _payload_str;

        /**
        * Context
        */
        hllp_cnt_ctx_t *ctx;
    public:
        /**
        *  c++ constructor
        */
       HyperLogLogPlusPlus(){
            std::cout << "defualt c++ constructor"<<std::endl;
       }

        /**
        * Paramtertic constructor with serialized bitmap  
        */

       // HyperLogLogPlusPlus(string &serialized_object) : serialized(serialized_object){
       //      ctx = hllp_cnt_init(&serialized_object, serialized_object.length());
       //      std::cout<<"String " << std::endl;
       // }

        /**
        *   Parametric constructor with bitmap length specified
        */
       HyperLogLogPlusPlus(int payload){
            value = payload;
            std::cout<<"Setting payload int"<< value << std::endl;
       }

        /**
        *  c++ destructor
        */
        virtual ~HyperLogLogPlusPlus(){
            std::cout<<"destructor"<<std::endl;
        }

        virtual void __destruct()
        {
            std::cout << "__destruct" << std::endl;
        }
    
         /**
         *  php "constructor"
         *  @param  params integer type for _payload variable
         *  Creating empty native object - hllp_cnt_init(), which creates a context
         */
        void __construct(Php::Parameters &params)
        {
            if (params.size() == 1){
               std::cout<<"Invoking hllp_cnt_init() from library to be extended"<<std::endl;
               // new HyperLogLogPlusPlus((int)params[0]);
               ctx = hllp_cnt_init(NULL, 16);
               std::cout<< ctx << std::endl;
            }   
        }

        /**
        * Creating serialized object -> hllp_cnt_init function
        * @param[in] string to be passed in order to create such object
        */
        void HyperLogLogPlusPlusString(Php::Parameters &params){
            ctx = hllp_cnt_init(params[0], params[0].length());
            std::cout<<ctx<<std::endl;
        }

        //TODO ask about second parameter
        
        /**
        * Adding element to _payload instance
        * Method connected to native hllp_cnt_offer method
        */
        void offer(Php::Parameters &params){
            std::cout<<params[0]<<std::endl;
             _payload= hllp_cnt_offer(ctx, &params[0], sizeof(params[0]));
        }

        /**
        * Computing numerical amount and returning result
        * Native C method is hllp_cnt_card
        */
        Php::Value count(){
            std::cout<<hllp_cnt_card(ctx)<< std::endl;
            // std::cout<<ctx<<std::endl;
            return hllp_cnt_card(ctx);
        }

        /**
        * Merging this class instance with the instance given as a parameter
        * @params instance of class itself
        * Associated method in native C lib is hllp_cnt_merge
        */
        void merge(Php::Parameters &params){

        }

        /**
        * Merging class instances but in serialized mode
        * @params serialized object, string
        * Associated method in native C lib is hllp_merge_bytes
        */
        void mergeRaw(Php::Parameters &params){

        }

        /**
        * Background cating to integer type
        */
        long __toInteger()
        {
            return _payload_int;
        }


        //TODO delete it soon, just testing method
        Php::Value get_payload(Php::Parameters &params) 
        { 
            std::string bitmap = "noname12312dfasdasdasdc1edasazcfedvz";
            std::cout<<(int)params[0]<<std::endl;
            std::cout<<bitmap.length()<<std::endl;
            ctx = hllp_cnt_init(&bitmap, bitmap.length());
            std::cout << ctx << std::endl;
            return _payload_int;// = params.empty() ? 1 : (int)params[0];
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
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::__construct>("__construct",{
            Php::ByRef("int", Php::Type::Numeric)
        });


        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::get_payload>("get_payload", {
            Php::ByVal("int", Php::Type::Numeric)
        });
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::count>("count", {});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::offer>("offer", {
            Php::ByVal("object", Php::Type::String)
        });
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::HyperLogLogPlusPlusString>("HyperLogLogPlusPlusString",{
            Php::ByVal("_payload_str", Php::Type::String)
        });

        // add the class to the extension
        hyperExtension.add(std::move(hyperLogLogPlusPlus));


        // return the extension
        return hyperExtension;
    }
}