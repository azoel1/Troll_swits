.PHONY: help setup build-apk clean install

help:
	@echo "Cookie Clicker - APK Builder"
	@echo ""
	@echo "Available commands:"
	@echo "  make setup       - Install dependencies (Node.js, Cordova)"
	@echo "  make build-apk   - Build the APK"
	@echo "  make clean       - Clean build artifacts"
	@echo "  make install     - Install dependencies and build"

setup:
	@echo "Installing Cordova CLI..."
	npm install -g cordova@latest
	@echo "Creating Cordova project..."
	cordova create . com.azoel.cookieclicker "Cookie Clicker" || true
	@echo "Adding Android platform..."
	cd . && cordova platform add android || true

build-apk:
	@echo "Building APK..."
	mkdir -p www
	cp cookie-clicker.html www/index.html
	cordova build android --release

clean:
	@echo "Cleaning build artifacts..."
	rm -rf platforms/
	rm -rf plugins/
	rm -rf www/
	rm -rf node_modules/

install: setup build-apk
	@echo "✅ APK built successfully!"
	@echo "APK location: platforms/android/app/build/outputs/apk/release/"

release: build-apk
	@echo "📦 Build complete! Ready for GitHub release."
