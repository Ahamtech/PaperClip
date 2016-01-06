/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.3
Page {
    actions: [
        ActionItem {
            id: itext
            ActionBar.placement: ActionBarPlacement.InOverflow
            title: "text"
        }
    ]
    titleBar: TitleBar {
        onSelectedOptionChanged: {
         switch (selectedValue)
         {
             case "All":
                 break
                 deligate.sourceComponent=readabilitycomp

             case "Readability":
                 deligate.sourceComponent=readabilitycomp

             break
             case "Instapaper":
                 deligate.sourceComponent=instapapercomp

             break
             case "Pocket":
                 deligate.sourceComponent=pocketcomp

              break
         }
        }
        kind: TitleBarKind.Segmented
      options: [
          Option {
              text: "All"
              value: "All"
          },
          Option {
            text: "Readability"
            value: "readability"  
          },
          
          Option {
              text: "Instapaper"
              value: "Instapaper"
          },
          Option {
              text: "Pocket"
              value: "Pocket"
          }
      ]
              
            
    }
    Container {
        ControlDelegate {
            id: deligate
        }
        ListView {
            id: mainlist
            dataModel: ArrayDataModel {
            }
            
        }
    }attachedObjects: [
        ComponentDefinition {
            id: pocketcomp
            DropDown {
                onSelectedOptionChanged: {
                    switch (selectedOption.value)
                    {
                        case "pocketacton2":
                            break
                        case "pocketacton1":
                            break
                    }
                }
                title: "PocketDropdown"
                options: [
                    Option {
                        text: "pocketaction1"
                        value: "pocketacton1"
                    },
                    Option {
                        text: "pocketaction2"
                        value: "pocketacton2"
                    }
                ]
            }
        },
        ComponentDefinition {
            id: instapapercomp
            DropDown {
                onSelectedOptionChanged: {
                    switch (selectedOption.value)
                    {
                        case "instapaperacton1":
                            break
                        case "instapaperacton2":
                            break
                    }
                }
                title: "InstapaperDropdown"
                options: [
                    Option {
                        text: "InstaPaperaction1"
                        value: "instapaperacton1"
                    },
                    Option {
                        text: "Instapaperaction2"
                        value: "instapaperacton"
                    }
                
                ]
            }
        },
        ComponentDefinition {
                    id: readabilitycomp
                    DropDown {
                        onSelectedOptionChanged: {
                            switch (selectedOption.value)
                            {
                                case "readabilityacton1":
                                    break
                                case "readabilityacton2":
                                    break
                            }
                        }
                        title: "ReadbilityDropdown"
                        options: [
                            Option {
                                text: "readabilityaction1"
                                value: "readabilityacton1"
                            },
                            Option {
                                text: "readabilityaction2"
                                value: "readabilityacton2"
                            }
                        ]
                    }

        }
    ]
}


