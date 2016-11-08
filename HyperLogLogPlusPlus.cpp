#include "HyperLogLogPlusPlus.h"


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
         */
        void __construct(Php::Parameters &params)
        {
            // copy first parameter (if available)
            if (!params.empty()) _payload = params[0];

        }

};
//        HyperLogLogPlusPlus( int ) {
//         //utwórz pusty obiekt (hllp_cnt_init)
//        }

//       public function HyperLogLogPlusPlus( string ) {
//          // utwórz obiekt z serialiowanego (hllp_cnt_init)
//       }
//       public function offer( string ) {
//          // dodaj element do payload  (hllp_cnt_offer)
//       }
//       public function count() {
//          //zwróc liczebność obiektu (hllp_cnt_card)
//       }
//
//       public function merge( HyperLogLogPlusPlus ) {
//          //dodaj obiekt z paramteru do obecnego obiektu  (hllp_cnt_merge)
//       }
//
//       public function mergeRaw( string ) {
//         //dodaje obiekt ale w postaci serializowanej (hllp_merge_bytes)
//       }
//       public function toString() {
//          //zwróc string reprezentujący ten obiekt, tak żeby można go było podać do konstruktura i dostać ten sam wynik (hllp_cnt_get_bytes)
//       }




/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    //TODO how to include ccardlib library here
    //#include "my-C-code.h"


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
        static Php::Extension hyperExtension("hyperLogLog", "1.0");

        Php::Class<HyperLogLogPlusPlus> hyperLogLogPlusPlus("HyperLogLogPlusPlus");

//        hyperLogLogPlusPlus.m
//        counter.method<&Counter::increment> ("increment");
//        counter.method<&Counter::decrement> ("decrement");
//        counter.method<&Counter::value>     ("value");

        // add the class to the extension
        hyperExtension.add(std::move(hyperLogLogPlusPlus));

        // @todo    add your own functions, classes, namespaces to the extension

        // return the extension
        return hyperExtension;
    }
}