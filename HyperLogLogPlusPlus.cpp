#include "HyperLogLogPlusPlus.h"
extern "C"{
    #include "ccard-lib/include/ccard_common.h"
    #include "ccard-lib/include/hyperloglogplus_counting.h"
}
using namespace std;

class HyperLogLogPlusPlus : public Php::Base{
    private:
        int _payload;
        

    public:
        /**
        *  c++ constructor
        */
       HyperLogLogPlusPlus() = default;


        /**
        *  c++ destructor
        */
        virtual ~HyperLogLogPlusPlus() = default;

         /**
         *  php "constructor"
         *  @param  params integer type for _payload variable
         *  Creating empty native object - hllp_cnt_init()
         */
        void __construct(Php::Parameters &params)
        {
            std::cout << "Php default constructor" << std::endl;

            if (params.size() == 1) {
                _payload = params[0];
                std::cout<<"Invoking hllp_cnt_init() from library to be extended"<<std::endl;
                hllp_cnt_ctx_t *ctx = hllp_cnt_raw_init(NULL, 16);

                //TODO invoke C native method
            }
        }

        //TODO delete it soon, just testing method
        Php::Value get_payload(Php::Parameters &params) 
        { 
            std::cout<<params[0]<<std::endl;
            return _payload = params.empty() ? 1 : (int)params[0];
        }

        /**
        * Creating serialized object -> hllp_cnt_init function
        * @param[in] string to be passed in order to create such object
        */
        void HyperLogLogPlusPlusString(Php::Parameters &params){

        }

        /**
        * Adding element to _payload instance
        * Method connected to native hllp_cnt_offer method
        */
        void offer(Php::Parameters &params){
            std::cout<<params[0]<<std::endl;
        }

        /**
        * Computing numerical amount and returning result
        * Native C method is hllp_cnt_card
        */
        Php::Value count(){
            return "What should I return?";
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

        Php::Value toString(){
            return "so far no object to return";
        }

};


/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    //TODO how to include ccardlib library here
    
    //including headers of ccard-lib  


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
            Php::ByRef("payload", Php::Type::Numeric)
        });
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::get_payload>("get_payload", {});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::count>("count", {});
        hyperLogLogPlusPlus.method<&HyperLogLogPlusPlus::offer>("offer", {
            Php::ByVal("object", Php::Type::String)
        });

        // add the class to the extension
        hyperExtension.add(std::move(hyperLogLogPlusPlus));


        // return the extension
        return hyperExtension;
    }
}