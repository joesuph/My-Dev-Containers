# My Emscripten Dev Container

This is a dev container for webassembly development and testing with C and C++

## Steps for testing and development

Emscripten steps were taken used from [here](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html#interacting-with-code-binding-cpp)

### 1. Run web server
1. Open new terminal in vscode ( Ctrl+shift+` )
2. Start server with

        cd server && node server.js
3. Place website files in public folder
4. Open a webpage in browser at https://localhost:8080/fileName.html

### 2. Write C/C++ Code
1. Be aware that in this example, only functions are exposed and all C/C++ functions must not be overloaded
2. Write C or C++ code (see [here](https://emscripten.org/docs/api_reference/index.html) for accessing webpage properties from code) 
3. Wrap Functions that you want to be able to expose with
__*extern "C" {    }*__

        //example:

        #include <iostream>

        extern "C" {
            int squareNum(int x)
            {
                return x*x;
            }
            
            void sayHello()
            {
                std::cout<<"Hello";
            }
        }
### 3. Compile Code
1. Have another terminal open in same directory as the .c or .cpp file that you want to compile
2. Use this format to compile your cpp file but replace fileName.cpp with the name of your cpp file and replace "_functionName1" and "functionName2" with your exposed functions but make sure functionNames are preceded by an underscore
    > emcc fileName.cpp -o fileName.html -s EXPORTED_FUNCTIONS='["_functionName1","_functionName2"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

    Example:

        emcc square_and_hello.cpp -o squareNum.html -s EXPORTED_FUNCTIONS='["_squareNum","_sayHello"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

    >Note: you may need to run this code a few times if files need to be created one after another
3. Move generated .wasm and .js files to server/public. You can delete the generate .html file or if you want to use it for testing you can move that over also.

### 4. Use code in a JS file

1. Include your page's javascript code to the html document then include the generated .js file.

        <!--example --->
        <script src="myScript.js"></script>

        <script async type="text/javascript" src="generated.js"></script>
    This will load the webassembly for you.
2. Make sure the wasm was loaded completely before you try to access the functions. 

        //myScript.js

        Module['onRuntimeInitialized'] = function() {
            console.log("wasm loaded ");
            
            //Here you can write or call code that references the wasm
        }
3. Call the function that you want to call with *Module*

        //myScript.js

        Module['onRuntimeInitialized'] = function() {
            console.log("wasm loaded ");
            
            Module._squareNum(2);

            Module._sayHello();
        }
4. *[Optional]* Wrap the functions in javascript functions and call them

        //myScript.js

        Module['onRuntimeInitialized'] = function() {
            console.log("wasm loaded ");
            
                /*  
                js_function = Module.cwrap('c_function_name', 'return_type', ['arg_1_type','arg_2_type'])
                */
                
                var squareNum = Module.cwrap('squareNum', 'int', ['int'])
                var sayHello = Module.cwrap('sayHello', 'void', ['void'])

                squareNum()
                sayHello()
        }
