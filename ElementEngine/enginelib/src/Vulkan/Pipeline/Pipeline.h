//
// Created by lbondi7 on 04/11/2020.
//

#pragma once

namespace Element {

    class Pipeline {

        virtual void init() = 0;
        virtual void destroy() = 0;
        virtual void flush() = 0;


    };
}

 //VULKANTEST_PIPELINE_H
