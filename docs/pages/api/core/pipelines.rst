Pipelines and Pipeline Configuration
====================================

This section describes the use of Thallium's cross-API **pipeline objects**.


*****


Types
-----


Objects
^^^^^^^

.. doxygentypedef:: TL_Pipeline_t


Descriptors
^^^^^^^^^^^

.. doxygenstruct:: TL_PipelineDescriptor_t
    :members:
.. doxygenstruct:: TL_PipelineRasterizerDescriptor_t
    :members:
.. doxygenstruct:: TL_PipelineDepthTestDescriptor_t
    :members:


Enums
^^^^^

.. doxygenenum:: TL_PipelineType_t


*****


Functions
---------

.. doxygenfunction:: TL_PipelineCreate
.. doxygenfunction:: TL_PipelineDestroy


*****


Pipeline configuration options
------------------------------

.. doxygenenum:: TL_CompareOp_t
.. doxygenenum:: TL_CullModeFlags_t
.. doxygenenum:: TL_PrimitiveTopology_t
.. doxygenenum:: TL_PolygonMode_t
