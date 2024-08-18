//
//  ContentView.swift
//  swift_ui
//
//  Created by comoco on 2024/8/18.
//

import SwiftUI

struct ContentView: View {
    
    @State private var buttonPressCount = 0
    
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundStyle(.tint)
            Button(action: {
                            // Increase the count when the button is pressed
                            buttonPressCount += 1
                let my_objc = MyObjectiveCClass();
                my_objc.someMethod();
                
                        }) {
                            // The label of the button
                            Text("Press Me")
                                .padding()
                                .background(Color.blue)
                                .foregroundColor(.white)
                                .cornerRadius(10)
                        }
        }
        .padding()
    }
}

#Preview {
    ContentView()
}
