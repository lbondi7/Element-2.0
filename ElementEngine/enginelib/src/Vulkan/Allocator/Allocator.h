//
// Created by lbondi7 on 25/11/2020.
//

#pragma once
#include "VknConstants.h"

namespace Element{
    struct VknBuffer;

    struct VknMemoryData{
        VkDeviceMemory memory;
        VkDeviceSize currentOffset;
        VkDeviceSize maxSize = 268435456;
        VkDeviceSize size = 0;
        uint32_t memoryType = 0;
        VkMemoryPropertyFlags properties;
        uint32_t id = 0;
    };

    struct VknBufferData{
        VknMemoryData* memoryData;
        VkBuffer vk_buffer;
        VkDeviceSize offset;
        VkDeviceSize maxSize = 67108864;
        VkDeviceSize size;
        VkBufferUsageFlags usage;
        uint32_t id;
        void* mapped = nullptr;
    };

    class Allocator {
        public:

        static Allocator &Get() {
            static Allocator alloc;
            return alloc;
        }

        void Allocate(VkDescriptorBufferInfo& bufferInfo, VkDeviceMemory& memory, VkDeviceSize& offset, VkDeviceSize
        size, VkBufferUsageFlags
        usage, VkMemoryPropertyFlags properties);
        void deallocate();


        private:
        Allocator();
        virtual ~Allocator();

        VknBufferData& getBuffer(VkDescriptorBufferInfo& descInfo, VkDeviceSize& offset,
                                 VkDeviceSize size, VkBufferUsageFlags usage,
                                 VkMemoryPropertyFlags properties);


        VknMemoryData &getMemory(const VkMemoryRequirements &memRequirements, VkMemoryPropertyFlags properties);

        std::vector<VknMemoryData> memoryPool;
        std::vector<VknBufferData> bufferPool;
        uint32_t availableMemoryID = 0;
        uint32_t availableBufferID = 0;
    };
};
//ELEMENTENGINE_ALLOCATOR_H
