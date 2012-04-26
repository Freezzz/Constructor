
{
   "difficulty" : 1.0,
   "game objects" : 
   [
      
      {
         "body" : 0,
         "inventory item" : 0,
         "isDeletable" : false,
         "isMovable" : false,
         "isRotatable" : false,
         "isStatic" : false
      }
   ],
   "game world" : 
   {
      "physics world" : 
      {
         "allowSleep" : true,
         "autoClearForces" : true,
         "body" : 
         [
            
            {
               "angle" : 0,
               "angularVelocity" : 0,
               "awake" : true,
               "fixture" : 
               [
                  
                  {
                     "density" : 0.1,
                     "friction" : 0.3000000119209290,
                     "polygon" : 
                     {
                        "vertices" : 
                        {
                           "x" : [ -0.781250, 0.781250, 0.781250, -0.781250 ],
                           "y" : [ -0.781250, -0.781250, 0.781250, 0.781250 ]
                        }
                     },
                     "restitution" : 0.3000000119209290
                  }
               ],
               "linearVelocity" : 0,
               "position" : 
               {
                  "x" : 5,
                  "y" : 3
               },
               "type" : 0 //static
            },
            
            {
               "angle" : 0,
               "angularVelocity" : 0,
               "awake" : true,
               "fixture" : 
               [
                  
                  {
                     "density" : 1,
                     "friction" : 0.3000000119209290,
                     "polygon" : 
                     {
                        "vertices" : 
                        {
                           "x" : [ -0.06250, 0.06250, 0.06250, -0.06250 ],
                           "y" : [ -0.06250, -0.06250, 0.06250, 0.06250 ]
                        }
                     },
                     "restitution" : 0.3000000119209290,
                     "sensor" : true
                  }
               ],
               "linearVelocity" : 0,
               "name" : "null body",
               "position" : 
               {
                  "x" : 16.0,
                  "y" : 12.0
               },
               "type" : 0 //static
            },
            
            {
               "angle" : 0,
               "angularVelocity" : 0,
               "awake" : true,
               "fixture" : 
               [
                  
                  {
                     "friction" : 0.2000000029802322,
                     "polygon" : 
                     {
                        "vertices" : 
                        {
                           "x" : 
                           [
                              15.99899959564209,
                              16.00099945068359,
                              16.00099945068359,
                              15.99899959564209
                           ],
                           "y" : [ -11.06250, -11.06250, 11.06250, 11.06250 ]
                        }
                     }
                  },
                  
                  {
                     "friction" : 0.2000000029802322,
                     "polygon" : 
                     {
                        "vertices" : 
                        {
                           "x" : 
                           [
                              -16.00099945068359,
                              -15.99899959564209,
                              -15.99899959564209,
                              -16.00099945068359
                           ],
                           "y" : [ -11.06250, -11.06250, 11.06250, 11.06250 ]
                        }
                     }
                  },
                  
                  {
                     "friction" : 0.2000000029802322,
                     "polygon" : 
                     {
                        "vertices" : 
                        {
                           "x" : [ -16.0, 16.0, 16.0, -16.0 ],
                           "y" : 
                           [
                              11.06149959564209,
                              11.06149959564209,
                              11.06350040435791,
                              11.06350040435791
                           ]
                        }
                     }
                  },
                  
                  {
                     "friction" : 0.2000000029802322,
                     "polygon" : 
                     {
                        "vertices" : 
                        {
                           "x" : [ -16.0, 16.0, 16.0, -16.0 ],
                           "y" : 
                           [
                              -11.06350040435791,
                              -11.06350040435791,
                              -11.06149959564209,
                              -11.06149959564209
                           ]
                        }
                     }
                  }
               ],
               "linearVelocity" : 0,
               "position" : 
               {
                  "x" : 16.0,
                  "y" : 12.0
               },
               "type" : 0 //static
            }
         ],
         "continuousPhysics" : true,
         "gravity" : 
         {
            "x" : 0,
            "y" : -9.800000190734863
         },
         "subStepping" : false,
         "warmStarting" : true
      }
   },
   "inventory items" : 
   [
      
      {
         "fixtureDef" : 
         {
            "density" : 1,
            "friction" : 0.3000000119209290,
            "isSensor" : false,
            "restitution" : 0.3000000119209290,
            "shape" : 
            {
               "polygon" : 
               {
                  "vertices" : 
                  {
                     "x" : [ -0.781250, 0.781250, 0.781250, -0.781250 ],
                     "y" : [ -0.781250, -0.781250, 0.781250, 0.781250 ]
                  }
               }
            }
         },
         "isDeletable" : true,
         "isMovable" : true,
         "isRotatable" : true,
         "isStatic" : false,
         "item sprite path" : "box.png",
         "object sprite path" : "Icon-Small-50.png",
         "type" : 0
      },
      
      {
         "fixtureDef" : 
         {
            "density" : 1,
            "friction" : 0.1000000014901161,
            "isSensor" : false,
            "restitution" : 0.50,
            "shape" : 
            {
               "circle" : 
               {
                  "center" : 0,
                  "radius" : 0.5156250
               }
            }
         },
         "isDeletable" : true,
         "isMovable" : true,
         "isRotatable" : false,
         "isStatic" : true,
         "item sprite path" : "fixed.png",
         "object sprite path" : "circle.png",
         "type" : 1
      },
      
      {
         "fixtureDef" : 
         {
            "density" : 1,
            "friction" : 0.3000000119209290,
            "isSensor" : false,
            "restitution" : 0.3000000119209290,
            "shape" : 
            {
               "polygon" : 
               {
                  "vertices" : 
                  {
                     "x" : [ -2.8593750, 2.8593750, 2.8593750, -2.8593750 ],
                     "y" : [ -0.2031250, -0.2031250, 0.2031250, 0.2031250 ]
                  }
               }
            }
         },
         "isDeletable" : true,
         "isMovable" : true,
         "isRotatable" : true,
         "isStatic" : false,
         "item sprite path" : "panel_btn.png",
         "object sprite path" : "panel.png",
         "type" : 2
      },
      
      {
         "isDeletable" : true,
         "isMovable" : true,
         "isRotatable" : true,
         "isStatic" : false,
         "item sprite path" : "spring_btn.png",
         "object sprite path" : "spring.png",
         "type" : 3
      },
      
      {
         "isDeletable" : true,
         "isMovable" : true,
         "isRotatable" : true,
         "isStatic" : false,
         "item sprite path" : "pin_btn.png",
         "object sprite path" : "pin.png",
         "type" : 4
      },
      
      {
         "isDeletable" : true,
         "isMovable" : true,
         "isRotatable" : true,
         "isStatic" : false,
         "item sprite path" : "glue_btn.png",
         "object sprite path" : "glue.png",
         "type" : 5
      }
   ],
   "lose conditions" : 0,
   "name" : "test level",
   "target" : 0,
   "theme" : "test theme",
   "win conditions" : 0
}
