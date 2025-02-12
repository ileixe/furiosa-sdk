ifndef OPENAPI_SPEC_URI
OPENAPI_SPEC_URI=https://api.furiosa.ai/openapi.yaml
endif

default: install

generate-openapi:
	@OPENAPI_GENERATOR_VERSION=3.0.2 \
	java -jar share/openapi-generator-cli-5.0.1.jar generate \
	--global-property apiTests=false,apiDocs=false,modelTests=false,modelDocs=false \
	-i ${OPENAPI_SPEC_URI} -g python -c share/default.json \
  -o ./;

install:
	MACOSX_DEPLOYMENT_TARGET=10.15 python ./setup.py develop

uninstall:
	pip uninstall -y --quiet furiosa-sdk-runtime; \
	pip uninstall -y --quiet furiosa-sdk-quantizer; \
	pip uninstall -y --quiet furiosa-sdk-model-validator; \
	pip uninstall -y --quiet furiosa-sdk-cli; \
	pip uninstall -y --quiet furiosa-sdk

install_full:
	pip install --quiet --use-feature=in-tree-build .[full]

build:
	python -m build

lint:
	pylint furiosa

upload_test:
	twine upload --repository testpypi dist/*

upload:
	twine upload dist/*

test:
	python -m unittest discover -v -s ./tests

doc:
	mkdir -p build/docs/src/furiosa
	cp -a ../furiosa-sdk-runtime/furiosa_sdk_runtime build/docs/src/furiosa/runtime
	cp -a ../furiosa-sdk-quantizer/furiosa_sdk_quantizer build/docs/src/furiosa/quantizer
	cp -a furiosa build/docs/src
	pdoc3 --html --force --output-dir build/docs/html build/docs/src/furiosa

clean:
	rm -rf build dist *.egg-info doc_srcs

update_version:
ifndef SDK_VERSION
	$(error "SDK_VERSION is not set")
endif
	find ../ -name setup.py -exec sed -i "s/version = '[^']*'/version = '${SDK_VERSION}'/" '{}' \;
	sed -i "s/furiosa-sdk-runtime ~= [^\n]*/furiosa-sdk-runtime ~= ${SDK_VERSION}/" ../furiosa-sdk-model-validator/setup.cfg
	sed -i "s/furiosa-sdk-quantizer ~= [^\n]*/furiosa-sdk-quantizer ~= ${SDK_VERSION}/" ../furiosa-sdk-model-validator/setup.cfg

