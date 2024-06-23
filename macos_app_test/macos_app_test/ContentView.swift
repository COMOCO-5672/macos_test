//
//  ContentView.swift
//  macos_app_test
//
//  Created by comoco on 2024/6/23.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
            VStack(alignment: .leading, spacing: 10) {
                Text("Welcome to SwiftUI")
                    .font(.title)
                Text("This is a VStack example.")
                    .font(.subheadline)
                HStack {
                    Text("Horizontal Stack")
                    Spacer()
                    Text("Right aligned")
                }
                Text("Tap me")
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(10)
                    .onTapGesture {
                        print("Button tapped")
                    }
            }
            .padding()
        }
}

#Preview {
    ContentView()
}
